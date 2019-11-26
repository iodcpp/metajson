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

IOD_SYMBOL(type);
IOD_SYMBOL(coordinates);
IOD_SYMBOL(features);
IOD_SYMBOL(properties);
IOD_SYMBOL(geometry);
IOD_SYMBOL(name);

using namespace iod;

int main(int argc, const char* argv[])
{

  auto json_geometry = make_metamap(s::type = std::string(),
                                    s::coordinates = std::vector<std::vector<std::vector<double>>>());

  auto json_canada = make_metamap(s::type = std::string(),
                                  s::features = { make_metamap(s::type = std::string(),
                                                               s::properties = make_metamap(_name = std::string()),
                                                               s::geometry = json_geometry)} );

  std::ifstream in(argv[1]);
  std::stringstream buffer;
  buffer << in.rdbuf();
  std::string file_content(buffer.str());
  
  std::cout << file_content.size() << std::endl;
  if (auto err = json_decode(file_content, json_canada))
    std::cout << "error: " << err.what << std::endl;

  std::string enc = json_encode(json_canada);
  std::cout << enc.size() << std::endl;
  std::cout << json_canada.features[1].geometry.coordinates[0][0][0] << std::endl;
  return 0;
}
