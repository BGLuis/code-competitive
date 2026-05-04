cmake_minimum_required(VERSION 3.10)
project(ProblemF LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Executables for the C and C++ solutions
add_executable(solution_c answers/c/solution.c)
add_executable(solution_cpp answers/cpp/solution.cpp)

# Enable testing
enable_testing()

# Function to add a test case
# Problem F has multiple solutions, so we need a checker.
# But for now, we just check if it compiles and runs.
# Real testing would involve multiplying the factors and comparing with X.
function(add_problem_test NAME INPUT_FILE EXE)
    add_test(NAME ${NAME}
             COMMAND sh -c "${EXE} < ${INPUT_FILE}")
endfunction()

# Add tests automatically from the input directory
file(GLOB INPUT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/input/F_*")
foreach(IN_FILE ${INPUT_FILES})
    get_filename_component(BASE_NAME ${IN_FILE} NAME)
    
    add_problem_test("test_cpp_${BASE_NAME}" ${IN_FILE} "$<TARGET_FILE:solution_cpp>")
    add_problem_test("test_c_${BASE_NAME}" ${IN_FILE} "$<TARGET_FILE:solution_c>")
endforeach()
