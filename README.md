iod::metajson
============================


iod::metajson is provides a fast and easy to use JSON
serializer/deserializer. The goal is to get as fast as a non-generic
specialized hand-written serializer, for example:

```c++
printf("{\"name\":\"%s\",\"age\":%i}", a.name, a.age);
```

While being easier and safer to use. The big difference of iod::metajson over
other serialization libraries is that it does not rely on a dynamic hashtable
to store the objects, but on plain static C++ objects.

*Limitation:* metajson only handles JSON objects with a *static structure known at compile time*.


Overview
============================

[iod::metamaps](https://github.com/iodcpp/metamap) are the easiest type to serialize:

```c++
IOD_SYMBOL(_name)
IOD_SYMBOL(_age)

auto m = make_metamap(_name = "John", _age = 12);
std::string str = json_encode(str);
```

And also to decode:

```c++
std::string input = R"json({"test1":12,"test2":"John"})json";
std::string str = json_decode(input, str);
```

The library also provides a non intrusive way to de/serialize any C++ objects. Since the library
has no way to introsect your object, you must pass it the description of your object :

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
json_object(_age,
            _name(json_key("last_name")))
```

Roadmap
=================

- Encode and decode UTF8
