#pragma once

#include <iod/metamap/metamap.hh>
#include <iod/symbol/ast.hh>
#include <tuple>

namespace iod
{

  template <typename T>
  struct json_object_;

  namespace impl
  {
    template <typename S, typename T>
    auto make_json_object_member(const assign_exp<S, T>& e)
    {
      return make_metamap(_name = e.left, _type = e.right);
    }

    template <typename S>
    auto make_json_object_member(const symbol<S>&)
    {
      return make_metamap(_name = S{});
    }


    template <typename M>
    auto metamap_to_json_object(M m)
    {
      auto tuple_maker = [] (auto&&... t) { return std::make_tuple(std::forward<decltype(t)>(t)...); };

      auto entities = map_reduce(m, [] (auto k, auto v) {
          return make_metamap(_name = k, _type = v);
        }, tuple_maker);
      return json_object_<decltype(entities)>{entities};
    }

    template <typename... E>
    auto json_object_to_metamap(const json_object_<std::tuple<E...>>& s)
    {
      auto make_kvs = [] (auto... elt)
        {
          return std::make_tuple((elt.name = elt.type)...);
        };

      auto kvs = std::experimental::apply(make_kvs, s.entities);
      return std::experimental::apply(make_metamap, kvs);
    }

    // template <typename S, typename... A>
    // auto make_json_object_member(const function_call_exp<S, A...>& e);
    // {
      // FIXME
      // return make_metamap(_name = e);
    // }
    
  }

  template <typename T>
  struct json_object_;

  template <typename O>
  struct json_vector_;
  

  template <typename E> constexpr auto json_is_vector(json_vector_<E>) ->  std::true_type { return {}; }
  template <typename E> constexpr auto json_is_vector(E) ->  std::false_type { return {}; }

  template <typename E> constexpr auto json_is_object(json_object_<E>) ->  std::true_type { return {}; }
  template <typename E> constexpr auto json_is_object(E) ->  std::false_type { return {}; }
  
}
