cmake_minimum_required(VERSION 3.10)

project(server VERSION 1.0)

add_executable(server server.cpp)

add_library(randomizer STATIC random.cpp)
add_library(serializer STATIC serializer.cpp)

target_link_libraries(server randomizer serializer)
