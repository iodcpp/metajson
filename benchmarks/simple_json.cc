#include <iostream>
#include "../metajson/decode_stringstream.hh"
#include "../metajson/metajson.hh"
#include <benchmark/benchmark.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "nlohmann_json.hpp"

//const char* json_str = "{\"test1\":12,\"test2\":12}";
const char* json_str = R"json({"test1":12,"test2":1223,"test3": "hello world"})json";

// static void stringstream(benchmark::State& state) {

//   long i = 0;
//   while (state.KeepRunning())
//   {
//     i = std::strtol(uint_str, nullptr, 10);
//   }

//   std::cout << i << std::endl;
// }

using namespace iod;

namespace s
{
  IOD_SYMBOL(test1);
  IOD_SYMBOL(test2);
  IOD_SYMBOL(test3);
}

static void iod_custom_stream(benchmark::State& state) {

  long i = 0;
  //std::stringstream ss = std::stringstream(std::string(json_str));
  auto obj = make_metamap(s::_test1 = 0, s::_test2 = 0, s::_test3 = std::string());
  while (state.KeepRunning())
  {
    decode_stringstream ss(json_str);
    //std::stringstream ss = std::stringstream(std::string(json_str));
    auto err = iod::json_decode(ss, obj);
    //std::cout << err.what << std::endl;
  }
  std::cout << obj.test3 << std::endl;
}
static void bench_rapidjson(benchmark::State& state) {

  long i = 0;
  //std::stringstream ss = std::stringstream(std::string(json_str));
  auto obj = make_metamap(s::_test1 = 0, s::_test2 = 0, s::_test3 = std::string());
  while (state.KeepRunning())
  {
    rapidjson::Document d;
    d.Parse<0>(json_str);
    // obj.test1 = d["test1"].GetInt();
    // obj.test2 = d["test2"].GetInt();
    // obj.test3 = d["test3"].GetString();
  }
}


static void bench_nlohmann_json(benchmark::State& state) {

  long i = 0;
  //std::stringstream ss = std::stringstream(std::string(json_str));
  auto obj = make_metamap(s::_test1 = 0, s::_test2 = 0, s::_test3 = std::string());
  while (state.KeepRunning())
  {
    auto d = nlohmann::json::parse(json_str);
    // obj.test1 = d["test1"];
    // obj.test2 = d["test2"];
    // obj.test3 = d["test3"];
  }
}

static void iod_stringstream(benchmark::State& state) {

  long i = 0;
  //std::stringstream ss = std::stringstream(std::string(json_str));
  auto obj = make_metamap(s::_test1 = 0, s::_test2 = 0, s::_test3 = std::string());
  while (state.KeepRunning())
  {
    //decode_stringstream ss(json_str);
    std::stringstream ss = std::stringstream(std::string(json_str));
    auto err = iod::json_decode(ss, obj);
    //std::cout << err.what << std::endl;
  }
}

BENCHMARK(iod_custom_stream);
BENCHMARK(bench_rapidjson);
BENCHMARK(bench_nlohmann_json);
BENCHMARK(iod_stringstream);

BENCHMARK_MAIN();
