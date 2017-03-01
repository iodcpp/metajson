cmake_minimum_required(VERSION 2.8)

project(metajson)

add_definitions(-std=c++1z)

enable_testing()

add_executable(json_decode tests/decode.cc)
add_test(json_decode json_decode)

add_executable(json_encode tests/encode.cc)
add_test(json_encode json_encode)
