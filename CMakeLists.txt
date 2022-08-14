cmake_minimum_required(VERSION 3.21)
project(my_learn_opengl)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(3rdparty)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
add_library(sb7 src/sb7/sb7.cpp)
target_link_libraries(sb7 PUBLIC glad)
target_include_directories(sb7 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/sb7)

set(LINK_LIBS ${CONAN_LIBS} glad sb7)

if (ENABLE_ASAN)
    message(STATUS "build with ASAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()

function(add_demo name)
    add_executable(${name} ${ARGN} lsan_suppressions.cpp)
    target_link_libraries(${name} PRIVATE ${LINK_LIBS})
endfunction()

add_demo(1_1_hello_window src/1_getting_start/1_1_hello_window.cpp)
add_demo(1_2_hello_triangle src/1_getting_start/1_2_hello_triangle.cpp)


add_demo(2_1 src/sb7/2_1.cpp)
add_demo(2_11 src/sb7/2_11.cpp)
