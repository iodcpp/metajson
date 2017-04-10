#include <iostream>
#include "../metajson/decode_stringstream.hh"
#include "../metajson/metajson.hh"
#include <benchmark/benchmark.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "nlohmann_json.hpp"

//const char* json_str = "{\"test1\":12,\"test2\":12}";
const char* json_str = R"json([0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9   ])json";

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
}

static void iod_custom_stream(benchmark::State& state) {

  long i = 0;
  //std::stringstream ss = std::stringstream(std::string(json_str));
  auto obj = std::vector<int>();
  while (state.KeepRunning())
  {
    obj.clear();
    decode_stringstream ss(json_str);
    //std::stringstream ss = std::stringstream(std::string(json_str));
    auto err = iod::json_decode(ss, obj);
    //std::cout << err.code << std::endl;
  }
  // std::cout << obj.size() << std::endl;
}


static void bench_rapidjson(benchmark::State& state) {

  long i = 0;
  //std::stringstream ss = std::stringstream(std::string(json_str));
  auto obj = std::vector<int>();
  while (state.KeepRunning())
  {
    obj.clear();
    rapidjson::Document d;
    d.Parse<0>(json_str);
    for (auto& x : d.GetArray())
      obj.push_back(int(x.GetInt()));
    // obj.test1 = d["test1"].GetInt();
    // obj.test2 = d["test2"].GetInt();
  }
  // std::cout << obj.size() << std::endl;
}


static void bench_nlohmann_json(benchmark::State& state) {

  long i = 0;
  //std::stringstream ss = std::stringstream(std::string(json_str));
  auto obj = std::vector<int>();
  while (state.KeepRunning())
  {
    obj.clear();
    auto d = nlohmann::json::parse(json_str);
    for (auto& x : d)
      obj.push_back(int(x));
    // obj.test1 = d["test1"];
    // obj.test2 = d["test2"];
  }
  // std::cout << obj.size() << std::endl;
}

static void iod_stringstream(benchmark::State& state) {

  long i = 0;
  //std::stringstream ss = std::stringstream(std::string(json_str));
  auto obj = std::vector<int>();
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
