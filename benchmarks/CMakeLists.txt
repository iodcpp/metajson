cmake_minimum_required(VERSION 2.8)

project(metajson)

add_definitions(-std=c++1z -O3 -DNDEBUG -march=native)
include_directories($ENV{HOME}/local/include ${CMAKE_BINARY_DIR}/include)
link_directories($ENV{HOME}/local/lib)


add_executable(parse_int parse_int.cc)
target_link_libraries(parse_int benchmark pthread)

add_executable(parse_float parse_float.cc)
target_link_libraries(parse_float benchmark pthread)

add_executable(simple_json simple_json.cc)
target_link_libraries(simple_json benchmark pthread)

add_executable(decode_int_array decode_int_array.cc)
target_link_libraries(decode_int_array benchmark pthread)

add_executable(decode_float_array decode_float_array.cc)
target_link_libraries(decode_float_array benchmark pthread)

add_executable(decode_float_array_iod decode_float_array_iod.cc)
add_executable(decode_float_array_rapidjson decode_float_array_rapidjson.cc)
add_executable(decode_float_array_nlo decode_float_array_nlo.cc)
