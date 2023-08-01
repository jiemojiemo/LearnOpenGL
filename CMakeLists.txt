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
    add_executable(${name} ${ARGN} ${PROJECT_SOURCE_DIR}/lsan_suppressions.cpp)
    target_link_libraries(${name} PRIVATE ${LINK_LIBS})
    target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/include)
endfunction()

add_subdirectory(src/1.getting_started)



