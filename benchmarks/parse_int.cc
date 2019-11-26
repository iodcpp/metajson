#include <iostream>
#include "../metajson/decode_stringstream.hh"
#include <benchmark/benchmark.h>

const char* uint_str = "232084905";

static void strtol(benchmark::State& state) {

  long i = 0;
  while (state.KeepRunning())
  {
    i = std::strtol(uint_str, nullptr, 10);
  }

}

static void parse_uint(benchmark::State& state) {

  long i = 0;
  while (state.KeepRunning())
  {
    iod::parse_uint(&i, uint_str, nullptr);
  }

}

BENCHMARK(parse_uint);
BENCHMARK(strtol);

BENCHMARK_MAIN();
