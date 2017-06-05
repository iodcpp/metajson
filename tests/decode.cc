#include <iod/metajson/metajson.hh>
#include <cassert>

namespace s
{
  IOD_SYMBOL(test1)
  IOD_SYMBOL(test2)
}

int main()
{
  { // Simple deserializer.
    std::string input = R"json({"test1":12,"test2":"John"})json";

    auto obj = iod::make_metamap(s::_test1 = int(),
                                 s::_test2 = std::string());

    iod::json_decode(input, obj);
    assert(obj.test1 == 12);
    assert(obj.test2 == "John");
  }

  #if 0

  { // Double quote escape.
    std::string input = R"json({"test1":12,"test2":"John\""})json";

    auto obj = iod::make_metamap(s::_test1 = int(),
                                 s::_test2 = std::string());
    
    iod::json_decode(input, obj);
    assert(iod::json_encode(obj) == input);

  }

  { // Double quote escape.
    std::string input = R"json({"test1":12,"test2":"John\\\""})json";

    auto obj = iod::make_metamap(s::_test1 = int(),
                                 s::_test2 = std::string());
    
    iod::json_decode(input, obj);
    assert(iod::json_encode(obj) == input);
  }

  { // json key.
    std::string input = R"json({"test1":12,"name":"John"})json";

    auto obj = iod::make_metamap(s::_test1 = int(),
                                 s::_test2 = std::string());

    json_object(s::_test1, s::_test2(iod::json_key("name"))).decode(input, obj);

    assert(obj.test1 == 12);
    assert(obj.test2 == "John");
  }
  
  {
    // plain vectors.
    std::string input = R"json([1,2,3,4])json";
    
    std::vector<int> v;
    auto err = iod::json_decode(input, v);
    assert(!err);
    assert(v.size() == 4);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    assert(v[3] == 4);
  }

  #endif
}
