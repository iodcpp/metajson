#pragma once

#include <sstream>

#include <iod/metajson/encoder.hh>
#include <iod/metajson/utils.hh>

namespace iod
{

  // Frontend.
  // json_object(...).decode(input, obj);
  // json_object(...).decode(input);

  // json_object(...).encode(output, obj);
  // json_object(...).encode(output, map);

  // json_object(...).encode(obj) -> string;
  // json_object(...).encode(map) -> string;
  
  // json_decode(input, map);
  // json_encode(ouput, map);
  // json_encode(map) -> string


  // Backend.
  // json_encode(output_stream, obj, members);
  // json_decode(output_stream, obj, members);

  // metamap_to_json_object
  // json_object_to_metamap
  // make_json_object_member OK.

  template <typename ...Ms>
  struct metamap;
  
  template <typename T>
  struct json_object_
  {

  public:

    template <typename C, typename O>
    void encode(C& output, O&& obj) const
    {
      return impl::json_encode(output, std::forward<O>(obj), *this);
    }

    template <typename C, typename... M>
    void encode(C& output, const metamap<M...>& obj) const
    {
      return impl::json_encode(output, obj, *this);
    }
      
    template <typename O>
    std::string encode(O obj) const
    {
      std::stringstream ss;
      impl::json_encode(ss, std::forward<O>(obj), *this);
      return ss.str();
    }

    template <typename... M>
    std::string encode(const metamap<M...>& obj) const
    {
      std::stringstream ss;
      impl::json_encode(ss, obj, *this);
      return ss.str();
    }
      
    // template <typename C, typename O>
    // void decode(C& input, O& obj) const
    // {
    //   return impl::json_decode(input, std::forward<O>(obj), members);
    // }

    // template <typename C, typename... M>
    // auto decode(C& input) const
    // {
    //   auto map = impl::json_object_to_metamap(*this);
    //   impl::json_decode(input, map, members);
    //   return map;
    // }

    T members;
  };
  
  template <typename... S>
  auto json_object(S&&... s)
  {
    auto members = std::make_tuple(impl::make_json_object_member(std::forward<S>(s))...);
    return json_object_<decltype(members)>{members};    
  }

  template <typename O>
  struct json_vector_
  {
    O object;
  };
    
  template <typename... S>
  auto json_vector(S&&... s)
  {
    auto obj = json_object(std::forward<S>(s)...);
    return json_vector_<decltype(obj)>{obj};
  }
  
  // template <typename C, typename... M>
  // auto json_decode(C& input, metamap<M...>& map)
  // {
  //   impl::metamap_to_json_object(map).decode(input, map);
  // }

  template <typename C, typename... M>
  auto json_encode(C& output, metamap<M...>& map)
  {
    impl::metamap_to_json_object(map).encode(output, map);
  }

  template <typename... M>
  auto json_encode(metamap<M...>& map)
  {
    return impl::metamap_to_json_object(map).encode(map);
  }
}
