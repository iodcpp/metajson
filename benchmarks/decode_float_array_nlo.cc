
#include "nlohmann_json.hpp"

//const char* json_str = "{\"test1\":12,\"test2\":12}";
const char* json_str = R"json([0.02e-3,1.024654e3,2.0246782e3,3.789402e3,4.02462e3,5.02e3,6.02e3,7.02e3,8.02e3,9.02e3,0.02e3,1.02e3,2.02e3,3.02e3,4.02e3,5.02e3,6.02e3,7.02e3,8.02e3,9.456202e3,0.02e3,1.078942e3,2.02e3,3.02e3,4.02e3,5.02e3,6.02e3,7.02e3,8.02e3,9.02e3,0.02e3,1.02e3,2.02e3,  3.02e3  ,   4.02e3,5.02e3,6.02e3,7.0456782e3,8.02e3,9.02e3,0.02e3,1.02e3,2.02e3,3.02e3,4.057952e3,5.02e3,6.0779852e3,7.02e3,8.048972e3,9.048492e3,0.02e3,1.02e3,2.02e3,3.02e3,4.02e3,5.02e3,6.02e3,7.02e3,8.02e3,9.3949   ])json";

int main() {
  auto obj = std::vector<float>();
  auto d = nlohmann::json::parse(json_str);
  for (auto& x : d)
    obj.push_back(float(x));
}
