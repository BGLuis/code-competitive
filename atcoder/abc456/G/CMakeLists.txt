cmake_minimum_required(VERSION 3.10)
project(ProblemG LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Executables for the C and C++ solutions
add_executable(solution_c answers/c/solution.c)
add_executable(solution_cpp answers/cpp/solution.cpp)

# Enable testing
enable_testing()

# Function to add a test case
function(add_problem_test NAME INPUT_FILE OUTPUT_FILE EXE)
    add_test(NAME ${NAME}
             COMMAND sh -c "${EXE} < ${INPUT_FILE} | diff -w - ${OUTPUT_FILE}")
endfunction()

# Add tests automatically from the input/output directories
file(GLOB INPUT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/input/G_*")
foreach(IN_FILE ${INPUT_FILES})
    get_filename_component(BASE_NAME ${IN_FILE} NAME)
    set(OUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/output/${BASE_NAME}")
    
    if(EXISTS ${OUT_FILE})
        add_problem_test("test_cpp_${BASE_NAME}" ${IN_FILE} ${OUT_FILE} "$<TARGET_FILE:solution_cpp>")
        add_problem_test("test_c_${BASE_NAME}" ${IN_FILE} ${OUT_FILE} "$<TARGET_FILE:solution_c>")
    endif()
endforeach()
