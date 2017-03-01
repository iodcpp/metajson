#include <iod/metajson/metajson.hh>
#include <cassert>

namespace s
{
  IOD_SYMBOL(test1)
  IOD_SYMBOL(test2)
}

int main()
{

  {
    std::string input = R"json({"test1":12,"test2":"John"})json";

    auto obj = iod::make_metamap(s::_test1 = 12,
                                 s::_test2 = iod::string_view("John"));
  
    auto enc = iod::json_encode(obj);
    assert(enc == input);

    std::stringstream ss;
    iod::json_encode(ss, obj);
    assert(ss.str() == input);
  
    struct { int test1; std::string test2; } obj2{12, "John"};  
    assert(iod::json_object(s::_test1, s::_test2).encode(obj2) == input);

    ss.str("");
    iod::json_object(s::_test1, s::_test2).encode(ss, obj2);
    assert(ss.str() == input);
  }

  {
    // Arrays;
    std::string input = R"json({"test1":2,"test2":[{"test1":11,"test2":"Bob"},{"test1":12,"test2":"John"}]})json";
  
    struct A { int test1; std::string test2; };
    struct { int test1; std::vector<A> test2; } obj{2, { {11, "Bob"}, {12, "John"} }};

    assert(json_object(s::_test1, s::_test2 = json_vector(s::_test1, s::_test2)).encode(obj) == input);
  }
  
  // auto obj = iod::make_metamap(s::_test1 = 12,
  //                              s::_test2 = iod::string_view("John"));
  
  // Decode into a plain obj via the definition of an encoding schema.
  // struct { int test1; std::string test2; } obj2;
  // auto schema = json_schema(_test1(json_key("name"), optional(-1)),
  //                           _test2 = json_vector(_test3),
  //                           _city = json_object(_test3, _test1));
  
  // json_decode(input, obj2, schema);
  // auto enc = json_encode(obj2, schema);
  
  // assert(obj.test1 == 12);
  // assert(obj.test2 == "John");

  // auto enc = json_encode(obj);

  // assert(enc == input);
}
