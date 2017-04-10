cmake_minimum_required(VERSION 2.8)

project(metajson)

add_definitions(-std=c++1z -O3 -DNDEBUG -fopenmp=libomp -march=native -g)
include_directories($ENV{HOME}/local/include ${CMAKE_BINARY_DIR}/include)
link_directories($ENV{HOME}/local/lib)

add_executable(parse_int parse_int.cc)
target_link_libraries(parse_int benchmark pthread)

add_executable(simple_json simple_json.cc)
target_link_libraries(simple_json benchmark pthread)


add_executable(decode_int_array decode_int_array.cc)
target_link_libraries(decode_int_array benchmark pthread)

