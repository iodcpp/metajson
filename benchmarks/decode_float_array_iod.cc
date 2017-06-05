#include "../metajson/metajson.hh"

const char* json_str = R"json([0.02e-3,1.024654e3,2.0246782e3,3.789402e3,4.02462e3,5.02e3,6.02e3,7.02e3,8.02e3,9.02e3,0.02e3,1.02e3,2.02e3,3.02e3,4.02e3,5.02e3,6.02e3,7.02e3,8.02e3,9.456202e3,0.02e3,1.078942e3,2.02e3,3.02e3,4.02e3,5.02e3,6.02e3,7.02e3,8.02e3,9.02e3,0.02e3,1.02e3,2.02e3,  3.02e3  ,   4.02e3,5.02e3,6.02e3,7.0456782e3,8.02e3,9.02e3,0.02e3,1.02e3,2.02e3,3.02e3,4.057952e3,5.02e3,6.0779852e3,7.02e3,8.048972e3,9.048492e3,0.02e3,1.02e3,2.02e3,3.02e3,4.02e3,5.02e3,6.02e3,7.02e3,8.02e3,9.3949   ])json";

using namespace iod;

namespace s
{
  IOD_SYMBOL(test1);
  IOD_SYMBOL(test2);
}

int main()
{
  auto obj = std::vector<float>();
  return iod::json_decode(json_str, obj);
}


// static void bench_rapidjson(benchmark::State& state) {

//   long i = 0;
//   auto obj = std::vector<float>();
//   while (state.KeepRunning())
//   {
//     obj.clear();
//     rapidjson::Document d;
//     d.Parse<0>(json_str);
//     for (auto& x : d.GetArray())
//       obj.push_back(x.GetFloat());
//   }
// }


// static void bench_nlohmann_json(benchmark::State& state) {

//   long i = 0;
//   auto obj = std::vector<float>();
//   while (state.KeepRunning())
//   {
//     obj.clear();
//     auto d = nlohmann::json::parse(json_str);
//     for (auto& x : d)
//       obj.push_back(float(x));
//   }
// }

// BENCHMARK(iod_custom_stream);
// BENCHMARK(bench_rapidjson);
// BENCHMARK(bench_nlohmann_json);

// BENCHMARK_MAIN();
