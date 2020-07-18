cmake_minimum_required(VERSION 3.10)

project(client)

add_executable(client client.cpp)

add_library(deserializer deserializer.cpp)

target_link_libraries(client deserializer)