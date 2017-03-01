#pragma once

#include <experimental/tuple>
#include <experimental/string_view>
#include <iod/metamap/metamap.hh>
#include <iod/symbol/symbol.hh>
#include <iod/metajson/symbols.hh>
#include <iod/metajson/utils.hh>

namespace iod
{
  using std::experimental::string_view;

  template <typename T>
  struct json_object_;
  
  namespace impl
  {

    // Json encoder.
    // =============================================

    // Fixme escape strings.

    template <typename T, typename C>
    inline void json_encode_value(C& ss, const T& t) { ss << t; }

    template <typename C>
    inline void json_encode_value(C& ss, const char* s) { ss << '"' << s << '"'; }

    template <typename C>
    inline void json_encode_value(C& ss, const string_view& s) { ss << '"' << s << '"'; }
    
    template <typename C>
    inline void json_encode_value(C& ss, const std::string& s) { ss << '"' << s << '"'; }

    template <typename C, typename O, typename E>
    inline void json_encode(C& ss, O obj, const json_object_<E>& schema);
    
    template <typename T, typename C, typename E>
    inline void json_encode(C& ss, const std::vector<T>& array, const json_vector_<E>& schema)
    {
      ss << '[';
      for (const auto& t : array)
      {
        json_encode(ss, t, schema.object);
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
      
      auto encode_members = [&] (auto... e)
        {
          apply_each(encode_one_entity, e...);
        };

      std::experimental::apply(encode_members, schema.members);
      ss << '}';
    }
  }

}
