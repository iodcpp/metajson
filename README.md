iod::metajson
============================


iod::metajson is a C++17 JSON serializer/deserializer, designed for
ease of use and execution speed.

Unlike others serialization libraries, it does not rely on dynamic
hashtables to store objects, but on plain static C++ objects.  This
makes it faster to compile, faster to execute, and easier to
use.

**Features:**
  - Header only
  - UTF-8 support
  - Exception free
  - Small codebase: 1200 LOC (~10x smaller than nlohmann/json [1] and rapidjson [2])

**Limitations:**
  - metajson only handles JSON objects with a **static structure known at compile time**.
  - iod::metajson properly handle decoding and encoding UTF-8 but not the others UTF-{32|16} {big|little} endian encodings.
  - No explicit errors for ill-formatted json messsages.

**Performances:** Up to **9x** faster than nlohmann/json and **2x** faster than rapidjson [2]. Please report
if you find cases where metajson performs slower.

[1] https://github.com/nlohmann/json

[2] https://github.com/miloyip/rapidjson

Author
============================

Matthieu Garrigues https://github.com/matt-42


Installation
============================

```
wget https://raw.githubusercontent.com/iodcpp/metajson/master/metajson.hh
```

```c++
#include "metajson.hh"
```

Overview
============================

```c++
#include "metajson.hh"
#include <iostream>

namespace s
{
  IOD_SYMBOL(name)
  IOD_SYMBOL(age)
  IOD_SYMBOL(entry)
  IOD_SYMBOL(id)
}

int main ()
{
     

  std::string json_str;

  // C-structs
  struct A { int age; std::string name; };
  A obj{12, "John"};

  json_str = iod::json_object(s::_age, s::_name).encode(obj);
  json_object(s::_age, s::_name).decode(json_str, obj);

  std::cout << json_str << std::endl;
  // {"age":12,"name":"John"}

  // C++ vectors
  std::vector<int> v = {1,2,3,4};
  json_str = iod::json_encode(v);
  iod::json_decode(json_str, v);

  std::cout << json_str << std::endl;
  // [1,2,3,4]

  // Arrays of structs
  std::vector<A> array{ {12, "John"},  {2, "Alice"},  {32, "Bob"} };
  json_str = iod::json_vector(s::_age, s::_name).encode(array);
  
  std::cout << json_str << std::endl;
  // [{"age":12,"name":"John"},{"age":2,"name":"Alice"},{"age":32,"name":"Bob"}]
 
  // Nested structs
  struct B { int id; A entry; };
  B obj2{ 1, { 12, "John"}};
  json_str = iod::json_object(s::_id = int(), s::_entry = json_object(s::_age, s::_name)).encode(obj2);

  std::cout << json_str << std::endl;
  // {"id":1,"entry":{"age":12,"name":"John"}}



  // Metamap
  using iod::make_metamap;
  auto map = make_metamap(s::_age = 12, s::_name = std::string("John"));
  json_str = json_encode(map);
  
  std::cout << json_str << std::endl;
  // {"age":12,"name":"John"}
  
  json_decode(json_str, map);


  // Custom JSON key.
  std::cout << json_object(s::_age, s::_name(iod::json_key("last_name"))).encode(obj) << std::endl;
  // {"age":12,"last_name":"John"}

}

```
