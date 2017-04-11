#include <iostream>
#include "../metajson/decode_stringstream.hh"
#include <benchmark/benchmark.h>

const char* uint_str = "2000";

static void strtol(benchmark::State& state) {

  long i = 0;
  while (state.KeepRunning())
  {
    i = std::strtol(uint_str, nullptr, 10);
  }

  std::cout << i << std::endl;
}

static void parse_uint(benchmark::State& state) {

  long i = 0;
  while (state.KeepRunning())
  {
    iod::internal::parse_uint(&i, uint_str, nullptr);
  }

  std::cout << i << std::endl;
}

BENCHMARK(parse_uint);
BENCHMARK(strtol);

BENCHMARK_MAIN();
