iod::metajson
============================


iod::metajson is a C++17 JSON serializer/deserializer, designed for
ease of use and execution speed.

Unlike others serialization libraries, it does not rely on dynamic
hashtables to store objects, but on plain static C++ objects.

**Features:**
  - UTF-8 support
  - Exception free
  - Small codebase: 1200 LOC (~10x smaller than nlohmann/json [1] and rapidjson [2])

**Limitations:**
  - metajson only handles JSON objects with a **static structure known at compile time**.
  - iod::metajson properly handle decoding and encoding UTF-8 but not the others UTF-{32|16} {big|little} endian encodings.

**Performances:** Up to **9x** faster than nlohmann/json and **2x** faster than rapidjson [2].

[1] https://github.com/nlohmann/json
[2] https://github.com/miloyip/rapidjson

Installation
============================

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


  // Metamaps

  iod::json_encode(iod::metamap)

}

```

[iod::metamaps](https://github.com/iodcpp/metamap) are the easiest type to serialize:

```c++
IOD_SYMBOL(_name)
IOD_SYMBOL(_age)

auto m = make_metamap(_name = "John", _age = 12);
std::string str = json_encode(m);
```

And also to decode:

```c++
std::string input = R"json({"age":12,"name":"John"})json";
json_decode(input, m);
```


And also to deserialize them:

```c++
json_object(_age, _name).decode(input, obj);
```

Vectors are also easy to handle:

```c++
std::vector<A> array;
json_vector(_age, _name).decode(input, obj);
```

And nested structures too:
```c++
std::vector<A> array;
json_object(_age, _name, _parents = json_array(_age, _name)).decode(input, obj);
```


If the member of the object does not match a given JSON key, you can
specify it with the json_key attribute in the description of your object:

```c++
json_object(_age, _name(json_key("last_name")))
```

Roadmap
=================

- Replace std::stringstream with a faster alternative
