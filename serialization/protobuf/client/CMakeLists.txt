cmake_minimum_required(VERSION 3.10)
project(protoclient VERSION 1.0)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../message.proto)

add_executable(client client.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(client ${Protobuf_LIBRARIES})