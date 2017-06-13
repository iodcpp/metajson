// Canada.json
// { "type": "FeatureCollection",
//   "features": [
// {
//     "type": "Feature",
// "properties": { "name": "Canada" },
// "geometry": {"type":"Polygon","coordinates":[[[-65.613616999999977,43.420273000000009],[-

#include <iostream>
#include <fstream>
#include "../metajson/metajson.hh"

#include <benchmark/benchmark.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "nlohmann_json.hpp"

namespace s
{
  IOD_SYMBOL(type);
  IOD_SYMBOL(coordinates);
  IOD_SYMBOL(features);
  IOD_SYMBOL(properties);
  IOD_SYMBOL(geometry);
  IOD_SYMBOL(name);
}

using namespace iod;

std::string json_str;

void load_file()
{  
  std::ifstream in("canada.json");
  std::stringstream buffer;
  buffer << in.rdbuf();
  json_str = std::string(buffer.str());
}


static void metajson(benchmark::State& state) {

  load_file();
  auto json_geometry = make_metamap(s::_type = std::string(),
                                    s::_coordinates = std::vector<std::vector<std::vector<float>>>());

  auto json_canada = make_metamap(s::_type = std::string(),
                                  s::_features = { make_metamap(s::_type = std::string(),
                                                              s::_properties = make_metamap(_name = std::string()),
                                                                s::_geometry = json_geometry)} );
  while (state.KeepRunning())
  {
    iod::json_decode(json_str, json_canada);
  }
}

static void bench_rapidjson(benchmark::State& state) {
  
  load_file();

  auto obj = std::vector<std::vector<std::vector<float>>>();
  int cpt = 0;
  while (state.KeepRunning())
  {
    obj.clear();
    rapidjson::Document d;
    d.Parse<0>(json_str.c_str());
    const rapidjson::Value& features = d["features"];
    cpt = 0;
    for (auto& x : features.GetArray())
    {
      obj.resize(obj.size() + 1);
      for (auto& y : x["geometry"]["coordinates"].GetArray())
      {
        obj.back().resize(obj.back().size() + 1);
        
        for (auto& z : y.GetArray())
        {
          for (auto& a : z.GetArray())
          {
            obj.back().back().push_back(a.GetFloat());
            cpt++;
          }
        }
      }
    }
  }
  std::cout << cpt << std::endl;
}

static void bench_nlohmann_json(benchmark::State& state) {

  load_file();
  while (state.KeepRunning())
  {
    auto d = nlohmann::json::parse(json_str);
  }
}

BENCHMARK(bench_rapidjson);
BENCHMARK(bench_nlohmann_json);
BENCHMARK(metajson);

BENCHMARK_MAIN();
