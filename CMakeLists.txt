# clings: open this folder in CLion to get one run target per exercise.
# The lines between the GENERATED markers are written by `clings dev update`.
cmake_minimum_required(VERSION 3.20)
project(clings C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(CLINGS_SOLUTIONS "Add solution targets" OFF)

enable_testing()

# The runner.
file(GLOB CLINGS_SRC CONFIGURE_DEPENDS src/*.c)
add_executable(clings ${CLINGS_SRC})
target_compile_options(clings PRIVATE -Wall -Wextra -Wshadow)
target_compile_definitions(clings PRIVATE _DEFAULT_SOURCE _POSIX_C_SOURCE=200809L)
target_include_directories(clings PRIVATE include src)

# One target per exercise. EXCLUDE_FROM_ALL keeps "Build All" from compiling
# exercises that are meant to fail; CLion still indexes them and offers Run/Debug.
function(clings_add_target target main dir)
    set(srcs ${main})
    foreach(helper IN LISTS ARGN)
        list(APPEND srcs exercises/${dir}/${helper})
    endforeach()
    add_executable(${target} EXCLUDE_FROM_ALL ${srcs})
    target_include_directories(${target} PRIVATE include exercises/${dir})
    target_compile_options(${target} PRIVATE
        -Wall -Wextra -Wshadow -g -fsanitize=address,undefined -fno-sanitize-recover=all)
    target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    target_link_libraries(${target} PRIVATE m)
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/exercises)
endfunction()

function(clings_exercise name dir)
    clings_add_target(${name} exercises/${dir}/${name}.c ${dir} ${ARGN})
endfunction()

function(clings_solution name dir)
    if(CLINGS_SOLUTIONS)
        clings_add_target(${name}_sol solutions/${dir}/${name}.c ${dir} ${ARGN})
    endif()
endfunction()

# Unit tests of the runner, plus the end-to-end script.
set(CLINGS_LIB_SRC ${CLINGS_SRC})
list(FILTER CLINGS_LIB_SRC EXCLUDE REGEX "/src/main\\.c$")
file(GLOB CLINGS_UNIT_TESTS CONFIGURE_DEPENDS tests/unit/test_*.c)
foreach(test_src IN LISTS CLINGS_UNIT_TESTS)
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src} ${CLINGS_LIB_SRC})
    target_compile_definitions(${test_name} PRIVATE _DEFAULT_SOURCE _POSIX_C_SOURCE=200809L)
    target_include_directories(${test_name} PRIVATE include src)
    target_link_libraries(${test_name} PRIVATE m)
    add_test(NAME ${test_name} COMMAND ${test_name} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endforeach()
add_test(NAME cli
    COMMAND ${CMAKE_SOURCE_DIR}/tests/integration/test_cli.sh $<TARGET_FILE:clings>
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# --- BEGIN GENERATED (clings dev update) ---
clings_exercise(intro1 00_intro)
clings_solution(intro1 00_intro)
clings_exercise(intro2 00_intro)
clings_solution(intro2 00_intro)
# --- END GENERATED ---
