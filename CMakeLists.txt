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
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endfunction()

add_demo(1_1_hello_window src/1_1_hello_window.cpp)

add_demo(2_1_hello_triangle src/2_1_hello_triangle.cpp)
add_demo(2_2_hello_triangle_indexed src/2_2_hello_triangle_indexed.cpp)
add_demo(2_3_hello_triangle_exercise1 src/2_3_hello_triangle_exercise1.cpp)
add_demo(2_4_hello_triangle_exercise2 src/2_4_hello_triangle_exercise2.cpp)
add_demo(2_5_hello_triangle_exercise3 src/2_5_hello_triangle_exercise3.cpp)

add_demo(3_1_shaders_uniform src/3_1_shaders_uniform.cpp)
add_demo(3_2_shaders_interpolation src/3_2_shaders_interpolation.cpp)
add_demo(3_3_shaders_class src/3_3_shaders_class.cpp)
add_demo(3_5_shaders_exercise2 src/3_5_shaders_exercise2.cpp)
add_demo(3_6_shaders_exercise3 src/3_6_shaders_exercise3.cpp)

add_demo(4_1_textures src/4_1_textures.cpp src/stb_image.cpp)
add_demo(4_2_textures_combined src/4_2_textures_combined.cpp src/stb_image.cpp)
add_demo(4_4_textures_exercise2 src/4_4_textures_exercise2.cpp src/stb_image.cpp)
add_demo(4_5_textures_exercise3 src/4_5_textures_exercise3.cpp src/stb_image.cpp)


