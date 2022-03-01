cmake_minimum_required(VERSION 3.5)

project(ray_tracer)

file(GLOB SRC "src/*.cpp")

add_executable(ray_tracer ${SRC})

target_include_directories(ray_tracer
PUBLIC
${PROJECT_SOURCE_DIR}/include
)