#pragma once

#include <experimental/tuple>
#include <experimental/string_view>
#include <iod/metamap/metamap.hh>
#include <iod/symbol/symbol.hh>
#include <iod/metajson/symbols.hh>
#include <iod/metajson/utils.hh>
#include <iod/metajson/unicode.hh>

namespace iod
{
  using std::experimental::string_view;

  template <typename T>
  struct json_object_;
  
  namespace impl
  {

    // Json encoder.
    // =============================================

    template <typename T, typename C>
    inline void json_encode_value(C& ss, const T& t) { ss << t; }

    template <typename C>
    inline void json_encode_value(C& ss, const char* s) { utf8_to_json(s, ss); }

    template <typename C>
    inline void json_encode_value(C& ss, const string_view& s) { utf8_to_json(s, ss); }
    
    template <typename C>
    inline void json_encode_value(C& ss, const std::string& s) { utf8_to_json(s, ss); }

    template <typename C, typename O, typename E>
    inline void json_encode(C& ss, O obj, const json_object_<E>& schema);
    
    template <typename T, typename C, typename E>
    inline void json_encode(C& ss, const std::vector<T>& array, const json_vector_<E>& schema)
    {
      ss << '[';
      for (const auto& t : array)
      {
        if constexpr(decltype(json_is_vector(E{})){} or decltype(json_is_object(E{})){}) {
            json_encode(ss, t, schema.schema);
          }
        else
          json_encode_value(ss, t);
          
        if (&t != &array.back())
          ss << ',';
      }
      ss << ']';
    }

    template <typename C, typename O, typename E>
    inline void json_encode(C& ss, O obj, const json_object_<E>& schema)
    {
      ss << '{';
      bool first = true;

      auto encode_one_entity = [&] (auto e)
        {
          if (!first) { ss << ','; }
          first = false; 
          if constexpr(has_key(e, _json_key)) {
              json_encode_value(ss, e.json_key);
            }
          else
            json_encode_value(ss, symbol_string(e.name));
          ss << ':';

          if constexpr(has_key(e, _type)) {
              if constexpr(decltype(json_is_vector(e.type)){} or decltype(json_is_object(e.type)){}) {
                  return json_encode(ss, symbol_member_access(obj, e.name), e.type);
                }
              else
                json_encode_value(ss, symbol_member_access(obj, e.name));
            }
          else
            json_encode_value(ss, symbol_member_access(obj, e.name));
        };

      tuple_apply_each(encode_one_entity, schema.schema);
      ss << '}';
    }
  }

}
