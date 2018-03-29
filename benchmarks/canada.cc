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

IOD_SYMBOL(type);
IOD_SYMBOL(coordinates);
IOD_SYMBOL(features);
IOD_SYMBOL(properties);
IOD_SYMBOL(geometry);
IOD_SYMBOL(name);

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
  auto json_geometry = make_metamap(s::type = std::string(),
                                    s::coordinates = std::vector<std::vector<std::vector<float>>>());

  auto json_canada = make_metamap(s::type = std::string(),
                                  s::features = { make_metamap(s::type = std::string(),
                                                               s::properties = make_metamap(_name = std::string()),
                                                               s::geometry = json_geometry)} );

  while (state.KeepRunning())
  {
    iod::json_decode(json_str, json_canada);
  }
}

static void bench_rapidjson(benchmark::State& state) {
  
  load_file();

  auto json_geometry = make_metamap(s::type = std::string(),
                                    s::coordinates = std::vector<std::vector<std::vector<float>>>());

  auto json_canada = make_metamap(s::type = std::string(),
                                  s::features = { make_metamap(s::type = std::string(),
                                                               s::properties = make_metamap(_name = std::string()),
                                                               s::geometry = json_geometry)} );
  
  auto obj = std::vector<std::vector<std::vector<float>>>();
  int cpt = 0;
  while (state.KeepRunning())
  {
    obj.clear();
    rapidjson::Document d;
    d.Parse<0>(json_str.c_str());
    json_canada.type = d["type"].GetString();
    json_canada.features.clear();
    
    const rapidjson::Value& features = d["features"];
    cpt = 0;
    for (auto& x : features.GetArray())
    {
      auto& features_array = json_canada.features;
      features_array.resize(features_array.size() + 1);
      json_canada.features.back().type = x["type"].GetString();
      json_canada.features.back().properties.name = x["properties"].GetString();
      json_canada.features.back().geometry.type = x["geometry"]["type"].GetString();
      for (auto& y : x["geometry"]["coordinates"].GetArray())
      {
        auto& coordinates_1 = features_array.back().geometry.coordinates;
        coordinates_1.resize(coordinates_1.size() + 1);
        
        for (auto& z : y.GetArray())
        {
          auto& coordinates_2 = coordinates_1.back();
          coordinates_2.resize(coordinates_2.size() + 1);
          for (auto& a : z.GetArray())
          {
            auto& coordinates_3 = coordinates_2.back();
            coordinates_3.push_back(a.GetFloat());
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

BENCHMARK(metajson);
BENCHMARK(bench_rapidjson);
BENCHMARK(bench_nlohmann_json);

BENCHMARK_MAIN();
