cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ray_tracer)

file(GLOB SRC "src/*.cpp")

add_executable(ray_tracer ${SRC})

target_include_directories(ray_tracer
PUBLIC
${PROJECT_SOURCE_DIR}/include
)