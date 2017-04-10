iod::metajson
============================


iod::metajson is provides a fast and easy to use JSON
serializer/deserializer. The goal is to achieve the performance of non-generic
specialized hand-written C alternatives.

The big difference of iod::metajson over  other serialization libraries is that
it does not rely on a dynamic hashtable to store the objects, but on plain
static C++ objects. This makes it is safer and faster.

**Limitation:** metajson only handles JSON objects with a **static
  structure known at compile time**.

This is a work in progress. All the features described here are
working well, but more work need to be done to optimize string
formating/parsing.

iod::metajson properly handle decoding and encoding UTF-8 but not the
others UTF-{32|16} {big|little} endian encodings.


Overview
============================

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

The library also provides a non intrusive way to de/serialize any C++ objects. Since the library
has no way to introspect your object, you must pass it the description of its content:

```c++
struct A { int age; std::string name; };

A obj{12, "John"};
std::string str = json_object(_age, _name).encode(obj);
```
([More on symbols and how we access objects here](https://github.com/iodcpp/symbol))

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
