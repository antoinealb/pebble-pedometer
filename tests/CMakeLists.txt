cmake_minimum_required(VERSION 2.6)
project(pedometer-algorithm)
set(CMAKE_BUILD_TYPE Debug)


include_directories(
    ../worker_src/c/
)

list( APPEND CMAKE_CXX_FLAGS "-DTESTS -std=c++0x -Wall -Wextra")
list( APPEND CMAKE_C_FLAGS "-DTESTS -Wall -Wextra -Wno-char-subscripts -std=c99")

file(GLOB tests *.cpp)

add_executable(tests
    ${tests}
    ../worker_src/c/adaptive_threshold.c
    ../worker_src/c/lowpass.c
    ../worker_src/c/moving_window.c
    ../worker_src/c/pedometer.c
)

add_executable(pedometer
    ../pedometer_csv.c
    ../worker_src/c/adaptive_threshold.c
    ../worker_src/c/lowpass.c
    ../worker_src/c/moving_window.c
    ../worker_src/c/pedometer.c
)

target_link_libraries(
    tests
    m
    CppUTest
    CppUTestExt
)

# Run unit tests
add_custom_target(check ./tests -c DEPENDS tests)
