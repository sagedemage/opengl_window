cmake_minimum_required(VERSION 3.22 VERSION 0.1.0)

project(open_gl_demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(STANDARD_CXX_VERSION_FLAG "-std=c++20")
set(OPTIMIZE_FLAG "-O3")
set(WARNING_FLAGS "-Werror -Wpedantic -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STANDARD_CXX_VERSION_FLAG} ${OPTIMIZE_FLAG} ${WARNING_FLAGS}")

file(COPY music DESTINATION ${CMAKE_BINARY_DIR})

add_executable(open_gl_demo 
    src/main.cpp
)

target_link_libraries(open_gl_demo -lglfw -lGLEW -lGLU -lGL -lSDL2 -lSDL2_mixer)
