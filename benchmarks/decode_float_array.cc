#include <iostream>
#include "../metajson/decode_stringstream.hh"
#include "../metajson/metajson.hh"
#include <benchmark/benchmark.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "nlohmann_json.hpp"

//const char* json_str = "{\"test1\":12,\"test2\":12}";
const char* json_str = R"json([0.02e-3,1.024654e3,2.0246782e3,3.789402e3,4.02462e3,5.02e3,6.02e3,7.02e3,8.02e3,9.02e3,0.02e3,1.02e3,2.02e3,3.02e3,4.02e3,5.02e3,6.02e3,7.02e3,8.02e3,9.456202e3,0.02e3,1.078942e3,2.02e3,3.02e3,4.02e3,5.02e3,6.02e3,7.02e3,8.02e3,9.02e3,0.02e3,1.02e3,2.02e3,  3.02e3  ,   4.02e3,5.02e3,6.02e3,7.0456782e3,8.02e3,9.02e3,0.02e3,1.02e3,2.02e3,3.02e3,4.057952e3,5.02e3,6.0779852e3,7.02e3,8.048972e3,9.048492e3,0.02e3,1.02e3,2.02e3,3.02e3,4.02e3,5.02e3,6.02e3,7.02e3,8.02e3,9.3949   ])json";

// static void stringstream(benchmark::State& state) {

//   long i = 0;
//   while (state.KeepRunning())
//   {
//     i = std::strtol(uint_str, nullptr, 10);
//   }

//   std::cout << i << std::endl;
// }

using namespace iod;


static void iod_custom_stream(benchmark::State& state) {

  long i = 0;
  auto obj = std::vector<float>();
  while (state.KeepRunning())
  {
    obj.clear();
    decode_stringstream ss(json_str);
    auto err = iod::metajson::json_decode(ss, obj);
  }
}


static void bench_rapidjson(benchmark::State& state) {

  long i = 0;
  auto obj = std::vector<float>();
  while (state.KeepRunning())
  {
    obj.clear();
    rapidjson::Document d;
    d.Parse<0>(json_str);
    for (auto& x : d.GetArray())
      obj.push_back(x.GetFloat());
  }
}


static void bench_nlohmann_json(benchmark::State& state) {

  long i = 0;
  auto obj = std::vector<float>();
  while (state.KeepRunning())
  {
    obj.clear();
    auto d = nlohmann::json::parse(json_str);
    for (auto& x : d)
      obj.push_back(float(x));
  }
}

BENCHMARK(iod_custom_stream);
BENCHMARK(bench_rapidjson);
BENCHMARK(bench_nlohmann_json);

BENCHMARK_MAIN();
