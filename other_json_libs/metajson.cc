#include <iostream>
#include <iod/metajson/metajson.hh>

  IOD_SYMBOL(pi);
  IOD_SYMBOL(happy);
  IOD_SYMBOL(name);
  IOD_SYMBOL(answer);
  IOD_SYMBOL(list);
  IOD_SYMBOL(object);
  IOD_SYMBOL(everything);
  IOD_SYMBOL(currency);
  IOD_SYMBOL(value);


int main()
{
  auto o = iod::make_metamap
    (s::pi = 3.141,
     s::happy = true,
     s::name = "Nield",
     s::answer = iod::make_metamap(s::everything = 42),
     s::list = std::vector<int>{1,0,2},
     s::object = iod::make_metamap(s::currency = "USD",
                            s::value = 42.99)
                        );

  std::cout << iod::json_encode(o) << std::endl;
  iod::json_encode(std::cout, std::vector<int>{1,2,3});
  std::cout << std::endl;
}
