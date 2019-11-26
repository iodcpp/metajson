#include "metajson.hh"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

IOD_SYMBOL(e)
IOD_SYMBOL(E)
IOD_SYMBOL(s)
IOD_SYMBOL(U)
IOD_SYMBOL(u)
IOD_SYMBOL(b)
IOD_SYMBOL(a)

using bina_side = std::vector<std::tuple<std::string, std::string, std::vector<int>>>;

struct bina_depth
{
  std::string e;
  uint64_t E;
  std::string s;
  uint32_t U;
  uint32_t u;

  bina_side b;
  bina_side a;
};

using iod_json_side = iod::json_vector_<iod::json_tuple_<std::string, std::string, iod::json_vector_<int>>>;

int main(int argc, char **argv)
{
  const std::string input = R"json({"e":"depthUpdate","E":1530337676555,"s":"KEYBTC","U":1851892,"u":1851927,"b":[["0.00000375","970825.00000000",[]],["0.00000374","1314068.00000000",[]],["0.00000373","698996.00000000",[]],["0.00000370","801472.00000000",[]],["0.00000368","742657.00000000",[]],["0.00000367","557610.00000000",[]],["0.00000364","303787.00000000",[]],["0.00000363","144797.00000000",[]],["0.00000362","473956.00000000",[]],["0.00000358","207030.00000000",[]],["0.00000353","142593.00000000",[]],["0.00000344","579446.00000000",[]],["0.00000334","301832.00000000",[]]],"a":[["0.00000375","0.00000000",[]],["0.00000379","176609.00000000",[]],["0.00000381","368148.00000000",[]],["0.00000383","108264.00000000",[]],["0.00000384","156233.00000000",[]],["0.00000385","213546.00000000",[]],["0.00000387","296058.00000000",[]],["0.00000392","506788.00000000",[]],["0.00000393","196718.00000000",[]]]})json";

  const std::string input2 = R"json({"e":"depthUpdate","E":1530337676555,"s":"KEYBTC","U":1851892,"u":1851927,"b":[["0.00000375","970825.00000000",[]],["0.00000374","1314068.00000000",[]]]})json";

  bina_depth depth;

  auto err = iod::json_object(s::e, s::E, s::s, s::U, s::u, s::b = iod::json_vector_<iod::json_tuple_<std::string, std::string, iod::json_vector_<int>>> {}, s::a = iod::json_vector_<iod::json_tuple_<std::string, std::string, iod::json_vector_<int>>> {}).decode(input, depth);
  assert(!err);

  for (const auto &price : depth.b)
  {
    fprintf(stderr, "[%s] [%s]\n", std::get<0>(price).c_str(), std::get<1>(price).c_str());
  }

  return EXIT_SUCCESS;
}
