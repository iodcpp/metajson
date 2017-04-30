#include <iomanip>
#include <iostream>
#include "../metajson/decode_stringstream.hh"
#include <benchmark/benchmark.h>

//const char* float_str = "1.000003e-10";
const char* float_str = "1.000000001e3";

static void strtod(benchmark::State& state) {

  double i = 0;
  while (state.KeepRunning())
  {
    i = std::strtod(float_str, nullptr);
  }

  std::cout << std::fixed << std::setprecision(20) << i << std::endl;
}

static void parse_float(benchmark::State& state) {

  double i = 0;
  while (state.KeepRunning())
  {
    iod::internal::parse_float(&i, float_str, nullptr);
  }

  std::cout << std::fixed << std::setprecision(20) << i << std::endl;
}

BENCHMARK(parse_float);
BENCHMARK(strtod);

BENCHMARK_MAIN();
