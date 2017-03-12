#include <cassert>
#include <iod/metajson/metajson.hh>
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

  {
    // json_key
    std::string input = R"json({"test1":12,"name":"John"})json";

    auto obj = iod::make_metamap(s::_test1 = 12,
                                 s::_test2 = iod::string_view("John"));
  
    assert(input == iod::json_object(s::_test1, s::_test2(iod::json_key("name"))).encode(obj));
  }

  {
    // Nested array
    std::string input = R"json([{"test1":["test1":12]}])json";

    using s::_test1;
    typedef decltype(iod::make_metamap(_test1 = std::vector<decltype(iod::make_metamap(_test1 = int()))>())) elt;
    auto obj = std::vector<elt>();
    obj.push_back(iod::make_metamap(_test1 = { iod::make_metamap(_test1 = 12) }));
  }

  {
    // plain vectors.
    std::string input = R"json([1,2,3,4])json";
    assert(iod::json_encode(std::vector<int>{1,2,3,4}) == input);
  }
}
