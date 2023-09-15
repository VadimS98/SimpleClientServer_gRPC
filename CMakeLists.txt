cmake_minimum_required(VERSION 3.0)
project(example-grpc LANGUAGES CXX)

#message(FATAL_ERROR "${CMAKE_CURRENT_SOURCE_DIR}/App1")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/client" build_client)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/server" build_server)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/proto" build_proto)