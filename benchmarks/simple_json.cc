#include <iostream>
#include "../metajson/decode_stringstream.hh"
#include "../metajson/metajson.hh"
#include <benchmark/benchmark.h>

//const char* json_str = "{\"test1\":12,\"test2\":12}";
const char* json_str = R"json({"test1":12,"test2":1223})json";

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

static void custom(benchmark::State& state) {

  long i = 0;
  //std::stringstream ss = std::stringstream(std::string(json_str));
  auto obj = make_metamap(s::_test1 = 0, s::_test2 = 0);
  while (state.KeepRunning())
  {
    decode_stringstream ss(json_str);
    //std::stringstream ss = std::stringstream(std::string(json_str));
    auto err = iod::json_decode(ss, obj);
    //std::cout << err.what << std::endl;
  }
}

static void stringstream(benchmark::State& state) {

  long i = 0;
  //std::stringstream ss = std::stringstream(std::string(json_str));
  auto obj = make_metamap(s::_test1 = 0, s::_test2 = 0);
  while (state.KeepRunning())
  {
    //decode_stringstream ss(json_str);
    std::stringstream ss = std::stringstream(std::string(json_str));
    auto err = iod::json_decode(ss, obj);
    //std::cout << err.what << std::endl;
  }
}

BENCHMARK(custom);
BENCHMARK(stringstream);

BENCHMARK_MAIN();
