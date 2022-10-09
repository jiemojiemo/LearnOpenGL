cmake_minimum_required(VERSION 3.21)
project(my_learn_opengl)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(3rdparty)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(LINK_LIBS ${CONAN_LIBS} glad)

if (ENABLE_ASAN)
    message(STATUS "build with ASAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()

function(add_demo name)
    add_executable(${name} ${ARGN} lsan_suppressions.cpp)
    target_link_libraries(${name} PRIVATE ${LINK_LIBS})
endfunction()

add_demo(1_1_hello_window src/1_1_hello_window.cpp)
add_demo(2_1_hello_triangle src/2_1_hello_triangle.cpp)
add_demo(2_2_hello_triangle_indexed src/2_2_hello_triangle_indexed.cpp)
add_demo(2_3_hello_triangle_exercise1 src/2_3_hello_triangle_exercise1.cpp)


