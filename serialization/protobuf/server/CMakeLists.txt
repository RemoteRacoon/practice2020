cmake_minimum_required(VERSION 3.10)
project(protoserver VERSION 1.0)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../message.proto)

add_executable(server server.cpp ${PROTO_SRCS} ${PROTO_HDRS})
add_library(randomizer STATIC random.cpp)
target_link_libraries(server ${Protobuf_LIBRARIES})
target_link_libraries(server randomizer)