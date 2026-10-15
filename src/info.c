#include "info.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 32

enum vkind { V_STR, V_BOOL, V_INT, V_ARR };

struct value {
    enum vkind kind;
    char *str;
    bool b;
    long i;
    char **arr;
    size_t n_arr;
};

struct parser {
    const char *p;
    int line; /* 1-based line of p */
    char *err;
    size_t errlen;
};

static bool fail(struct parser *ps, int line, const char *fmt, ...) {
    char msg[200];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(msg, sizeof msg, fmt, ap);
    va_end(ap);
    if (ps->errlen)
        snprintf(ps->err, ps->errlen, "%d: %s", line, msg);
    return false;
}

static void value_free(struct value *v) {
    free(v->str);
    for (size_t i = 0; i < v->n_arr; i++)
        free(v->arr[i]);
    free(v->arr);
    memset(v, 0, sizeof *v);
}

static void skip_blanks(const char **p) {
    while (**p == ' ' || **p == '\t' || **p == '\r')
        (*p)++;
}

/* After a value: only blanks and an optional comment may remain on the line. */
static bool at_line_end(const char *p) {
    skip_blanks(&p);
    return *p == '\0' || *p == '\n' || *p == '#';
}

static void skip_line(struct parser *ps) {
    while (*ps->p && *ps->p != '\n')
        ps->p++;
    if (*ps->p == '\n') {
        ps->p++;
        ps->line++;
    }
}

/* Decode one escape; ps->p is at the char after the backslash. */
static bool decode_escape(struct parser *ps, struct buf *out) {
    char c = *ps->p;
    switch (c) {
    case '\\': case '"': buf_append(out, &c, 1); break;
    case 'n': buf_appends(out, "\n"); break;
    case 't': buf_appends(out, "\t"); break;
    default:
        return fail(ps, ps->line, "invalid escape sequence \\%c", c ? c : '0');
    }
    ps->p++;
    return true;
}

/* ps->p is just past the opening quote(s). Returns malloc'd decoded text. */
static bool parse_string(struct parser *ps, bool multi, char **out) {
    int start_line = ps->line;
    struct buf b;
    buf_init(&b);
    if (multi) {
        if (*ps->p == '\r' && ps->p[1] == '\n')
            ps->p++;
        if (*ps->p == '\n') {
            ps->p++;
            ps->line++;
        }
    }
    for (;;) {
        char c = *ps->p;
        if (c == '\0' || (!multi && c == '\n')) {
            buf_free(&b);
            return fail(ps, start_line, "unterminated string");
        }
        if (!multi && c == '"') {
            ps->p++;
            break;
        }
        if (multi && strncmp(ps->p, "\"\"\"", 3) == 0) {
            ps->p += 3;
            break;
        }
        ps->p++;
        if (c == '\\') {
            if (!decode_escape(ps, &b)) {
                buf_free(&b);
                return false;
            }
            continue;
        }
        if (c == '\n')
            ps->line++;
        buf_append(&b, &c, 1);
    }
    *out = b.data ? b.data : xstrdup("");
    return true;
}

static bool parse_array(struct parser *ps, struct value *v) {
    ps->p++; /* '[' */
    for (;;) {
        skip_blanks(&ps->p);
        if (*ps->p == ']') {
            ps->p++;
            return true;
        }
        if (*ps->p != '"')
            return fail(ps, ps->line, "expected string in array");
        ps->p++;
        char *s;
        if (!parse_string(ps, false, &s))
            return false;
        v->arr = realloc(v->arr, (v->n_arr + 1) * sizeof *v->arr);
        if (!v->arr)
            die("out of memory");
        v->arr[v->n_arr++] = s;
        skip_blanks(&ps->p);
        if (*ps->p == ',')
            ps->p++;
        else if (*ps->p != ']')
            return fail(ps, ps->line, "expected ',' or ']' in array");
    }
}

/* ps->p is at the first char of the value. Consumes through the value. */
static bool parse_value(struct parser *ps, struct value *v) {
    memset(v, 0, sizeof *v);
    const char *p = ps->p;
    if (*p == '"') {
        v->kind = V_STR;
        bool multi = strncmp(p, "\"\"\"", 3) == 0;
        ps->p += multi ? 3 : 1;
        return parse_string(ps, multi, &v->str);
    }
    if (*p == '[') {
        v->kind = V_ARR;
        if (!parse_array(ps, v)) {
            value_free(v);
            return false;
        }
        return true;
    }
    size_t n = strcspn(p, " \t\r\n#");
    if (n == 4 && strncmp(p, "true", 4) == 0) {
        v->kind = V_BOOL;
        v->b = true;
    } else if (n == 5 && strncmp(p, "false", 5) == 0) {
        v->kind = V_BOOL;
    } else {
        char *end;
        v->kind = V_INT;
        v->i = n ? strtol(p, &end, 10) : 0;
        if (n == 0 || end != p + n)
            return fail(ps, ps->line,
                        "invalid value (expected string, boolean, integer or array)");
    }
    ps->p += n;
    return true;
}

static bool want(struct parser *ps, const struct value *v, enum vkind k,
                 const char *key, const char *what) {
    if (v->kind == k)
        return true;
    return fail(ps, ps->line, "%s must be %s", key, what);
}

static bool valid_name(const char *s) {
    if (!*s)
        return false;
    for (; *s; s++)
        if (!isalnum((unsigned char)*s) && *s != '_')
            return false;
    return true;
}

static bool take_str(struct parser *ps, struct value *v, const char *key, char **dst) {
    if (!want(ps, v, V_STR, key, "a string"))
        return false;
    *dst = v->str;
    v->str = NULL;
    return true;
}

static bool set_top(struct parser *ps, struct info_file *f, const char *key,
                    struct value *v, int line) {
    if (!strcmp(key, "format_version")) {
        if (!want(ps, v, V_INT, key, "an integer"))
            return false;
        if (v->i != 1)
            return fail(ps, line, "unsupported format_version %ld", v->i);
        f->format_version = 1;
        return true;
    }
    char **dst = !strcmp(key, "welcome_message") ? &f->welcome_message
               : !strcmp(key, "final_message")   ? &f->final_message
                                                 : NULL;
    if (!dst)
        return fail(ps, line, "unknown key \"%s\"", key);
    free(*dst);
    *dst = NULL;
    return take_str(ps, v, key, dst);
}

static bool set_exercise(struct parser *ps, struct info_file *f, const char *key,
                         struct value *v, int line) {
    struct exercise_info *e = &f->exercises[f->n - 1];
    if (!strcmp(key, "name")) {
        if (e->name)
            return fail(ps, line, "duplicate key \"name\"");
        if (!want(ps, v, V_STR, key, "a string"))
            return false;
        if (!valid_name(v->str))
            return fail(ps, line, "invalid exercise name \"%s\" (use letters, digits, _)", v->str);
        if (strlen(v->str) > MAX_NAME_LEN)
            return fail(ps, line, "exercise name \"%s\" too long (max %d)", v->str, MAX_NAME_LEN);
        for (size_t i = 0; i + 1 < f->n; i++)
            if (!strcmp(f->exercises[i].name, v->str))
                return fail(ps, line, "duplicate exercise name \"%s\"", v->str);
        return take_str(ps, v, key, &e->name);
    }
    if (!strcmp(key, "dir"))
        return !e->dir ? take_str(ps, v, key, &e->dir)
                       : fail(ps, line, "duplicate key \"dir\"");
    if (!strcmp(key, "hint"))
        return !e->hint ? take_str(ps, v, key, &e->hint)
                        : fail(ps, line, "duplicate key \"hint\"");
    if (!strcmp(key, "stdin")) {
        free(e->stdin_text);
        e->stdin_text = NULL;
        return take_str(ps, v, key, &e->stdin_text);
    }
    if (!strcmp(key, "strict") || !strcmp(key, "skip_check_unsolved")) {
        if (!want(ps, v, V_BOOL, key, "a boolean"))
            return false;
        *(key[0] == 's' && key[1] == 't' ? &e->strict : &e->skip_check_unsolved) = v->b;
        return true;
    }
    if (!strcmp(key, "sources")) {
        if (!want(ps, v, V_ARR, key, "an array of strings"))
            return false;
        for (size_t i = 0; i < e->n_sources; i++)
            free(e->sources[i]);
        free(e->sources);
        e->sources = v->arr;
        e->n_sources = v->n_arr;
        v->arr = NULL;
        v->n_arr = 0;
        return true;
    }
    return fail(ps, line, "unknown key \"%s\"", key);
}

/* Check required keys of the exercise being closed and compute its path. */
static bool finish_exercise(struct parser *ps, struct info_file *f, int header_line) {
    if (!f->n)
        return true;
    struct exercise_info *e = &f->exercises[f->n - 1];
    const char *missing = !e->name ? "name" : !e->dir ? "dir" : !e->hint ? "hint" : NULL;
    if (missing)
        return fail(ps, header_line, "exercise is missing required key \"%s\"", missing);
    size_t n = strlen("exercises//.c") + strlen(e->dir) + strlen(e->name) + 1;
    e->path = xmalloc(n);
    snprintf(e->path, n, "exercises/%s/%s.c", e->dir, e->name);
    return true;
}

static bool start_exercise(struct info_file *f) {
    struct exercise_info *grown = realloc(f->exercises, (f->n + 1) * sizeof *grown);
    if (!grown)
        die("out of memory");
    f->exercises = grown;
    memset(&f->exercises[f->n++], 0, sizeof *grown);
    return true;
}

static bool parse_header(struct parser *ps, struct info_file *f, int *header_line) {
    int line = ps->line;
    if (strncmp(ps->p, "[[exercises]]", 13) != 0 || !at_line_end(ps->p + 13))
        return fail(ps, line, "unsupported table header (only [[exercises]] is allowed)");
    if (!finish_exercise(ps, f, *header_line))
        return false;
    *header_line = line;
    start_exercise(f);
    skip_line(ps);
    return true;
}

static bool parse_pair(struct parser *ps, struct info_file *f) {
    int line = ps->line;
    const char *p = ps->p;
    size_t klen = strspn(p, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_");
    char key[64];
    if (klen == 0 || klen >= sizeof key)
        return fail(ps, line, "expected 'key = value'");
    memcpy(key, p, klen);
    key[klen] = '\0';
    ps->p = p + klen;
    skip_blanks(&ps->p);
    if (*ps->p != '=')
        return fail(ps, line, "expected '=' after key \"%s\"", key);
    ps->p++;
    skip_blanks(&ps->p);
    struct value v;
    if (!parse_value(ps, &v))
        return false;
    bool ok = f->n ? set_exercise(ps, f, key, &v, line) : set_top(ps, f, key, &v, line);
    value_free(&v);
    if (!ok)
        return false;
    if (!at_line_end(ps->p))
        return fail(ps, ps->line, "unexpected text after value");
    skip_line(ps);
    return true;
}

static bool parse_all(struct parser *ps, struct info_file *f) {
    int header_line = 0;
    while (*ps->p) {
        skip_blanks(&ps->p);
        if (*ps->p == '\n' || *ps->p == '#' || *ps->p == '\0') {
            skip_line(ps);
            continue;
        }
        bool ok = *ps->p == '[' ? parse_header(ps, f, &header_line) : parse_pair(ps, f);
        if (!ok)
            return false;
    }
    if (!finish_exercise(ps, f, header_line))
        return false;
    if (f->format_version != 1)
        return fail(ps, 0, "missing format_version");
    if (!f->n)
        return fail(ps, 0, "no exercises");
    if (!f->welcome_message)
        f->welcome_message = xstrdup("");
    if (!f->final_message)
        f->final_message = xstrdup("");
    return true;
}

bool info_parse(const char *text, struct info_file *out, char *err, size_t errlen) {
    memset(out, 0, sizeof *out);
    struct parser ps = {text, 1, err, errlen};
    if (errlen)
        err[0] = '\0';
    if (parse_all(&ps, out))
        return true;
    info_free(out);
    return false;
}

bool info_load(const char *path, struct info_file *out, char *err, size_t errlen) {
    struct buf b;
    buf_init(&b);
    if (!file_read(path, &b)) {
        buf_free(&b);
        memset(out, 0, sizeof *out);
        if (errlen)
            snprintf(err, errlen, "info.toml: cannot read %s", path);
        return false;
    }
    char inner[400];
    bool ok = info_parse(b.data, out, inner, sizeof inner);
    buf_free(&b);
    if (!ok && errlen)
        snprintf(err, errlen, "info.toml:%s", inner);
    return ok;
}

void info_free(struct info_file *f) {
    for (size_t i = 0; i < f->n; i++) {
        struct exercise_info *e = &f->exercises[i];
        free(e->name);
        free(e->dir);
        free(e->path);
        free(e->hint);
        free(e->stdin_text);
        for (size_t j = 0; j < e->n_sources; j++)
            free(e->sources[j]);
        free(e->sources);
    }
    free(f->exercises);
    free(f->welcome_message);
    free(f->final_message);
    memset(f, 0, sizeof *f);
}

long info_find(const struct info_file *f, const char *name) {
    for (size_t i = 0; i < f->n; i++)
        if (!strcmp(f->exercises[i].name, name))
            return (long)i;
    return -1;
}

static size_t common_prefix(const char *a, const char *b) {
    size_t n = 0;
    while (a[n] && a[n] == b[n])
        n++;
    return n;
}

/* Lower is better: prefix matches, then substring matches, then by shared
 * prefix length. Returns -1 when the name is not a candidate at all. */
static long closeness(const char *cand, const char *name) {
    if (strncmp(cand, name, strlen(name)) == 0)
        return 0;
    if (strstr(cand, name))
        return 1;
    size_t cp = common_prefix(cand, name);
    return cp ? 1000 - (long)cp + 2 : -1;
}

void info_closest(const struct info_file *f, const char *name, struct buf *out) {
    size_t picked[3];
    size_t np = 0;
    for (; np < 3; np++) {
        long best = -1;
        size_t bi = 0;
        for (size_t i = 0; i < f->n; i++) {
            bool taken = false;
            for (size_t k = 0; k < np; k++)
                taken = taken || picked[k] == i;
            long s = taken ? -1 : closeness(f->exercises[i].name, name);
            if (s >= 0 && (best < 0 || s < best)) {
                best = s;
                bi = i;
            }
        }
        if (best < 0)
            break;
        picked[np] = bi;
        buf_appends(out, f->exercises[bi].name);
        buf_appends(out, "\n");
    }
}
