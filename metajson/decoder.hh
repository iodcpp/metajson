#pragma once

#include <functional>
#include <cstring>
#include <utility>
#include <experimental/string_view>
#include <iod/metamap/metamap.hh>
#include <iod/metajson/utils.hh>
#include <iod/metajson/unicode.hh>
#include <iod/metajson/error.hh>
#include <iod/metajson/decode_stringstream.hh>

namespace iod
{
  namespace impl
  {

    template <typename S>
    struct json_parser
    {
      inline json_parser(S&& s) : ss(s) {}
      inline json_parser(S& s) : ss(s) {}

      inline decltype(auto) peek() {
        return ss.peek();
      }
      inline decltype(auto) get() {
        return ss.get();
      }


      inline void skip_one() {
        ss.get();
      }
      
      inline bool eof() { return ss.eof(); }
      inline json_error eat(char c, bool skip_spaces = true) {
        if (skip_spaces)
          eat_spaces();

        char g = ss.get();
        if (g != c)
          return make_json_error("Unexpected char. Got '", char(g), "' expected ", c);
        return json_ok;
      }

      inline void eat_spaces()
      {
        while (ss.peek() >= 0 and ss.peek() < 33) ss.get();
      }

      template <typename X>
      struct JSON_INVALID_TYPE;

      // Integers and floating points.
      template <typename T>
      json_error fill(T& t) {

        if constexpr(std::is_floating_point<T>::value or
                     std::is_integral<T>::value or
                     std::is_same<T, iod::string_view>::value
          ) {
            ss >> t;
            if (ss.bad())
              return make_json_error("Ill-formated value.");
            return json_ok;
          }
        else
          // The JSON decoder only parses floating-point, integral and string types.
          return JSON_INVALID_TYPE<T>::error;
      }

      // Strings
      inline json_error fill(std::string& str)
      {
        eat_spaces();
        str.clear();
        return json_to_utf8(ss, str);
        //return json_ok;
      }
      
      S& ss;
    };

    template <typename P, typename O, typename S>
    json_error json_decode2(P& p, O& obj, S)
    {
      auto err = p.fill(obj);
      if (err) return err;
      else
        return json_ok;
    }
    
    template <typename P, typename O, typename S>
    json_error json_decode2(P& p, O& obj, json_vector_<S> schema)
    {
      bool first = true;
      auto err = p.eat('[');
      if (err) return err;

      p.eat_spaces();
      while (p.peek() != ']')
      {
        if (!first)
        {
          if ((err = p.eat(','))) return err;
        }
        first = false;

        obj.resize(obj.size() + 1);
        if ((err = json_decode2(p, obj.back(), obj.back()))) return err;
        p.eat_spaces();
      }
      
      if ((err = p.eat(']'))) return err;
      else
        return json_ok;
    }
    
    template <typename P, typename O, typename S>
    json_error json_decode2(P& p, O& obj, json_object_<S> schema)
    {
      json_error err;
      if ((err = p.eat('{'))) return err;

      struct attr_info { bool filled; const char* name; int name_len; std::function<json_error(P&)> parse_value; };
      constexpr int n_members = std::tuple_size<decltype(schema.schema)>();
      attr_info A[n_members];
      int i = 0;
      auto prepare = [&] (auto m) {
        A[i].filled = false;
        A[i].name = symbol_string(m.name);
        A[i].name_len = strlen(symbol_string(m.name));

        if constexpr(has_key(m, _json_key)) {
            A[i].name = m.json_key;
          }
        
        A[i].parse_value = [m,&obj] (P& p) {
          if (auto err = p.fill(symbol_member_access(obj, m.name))) return err;
          else return json_ok;
        };

        i++;
      };

      std::experimental::apply([&] (auto... m) { apply_each(prepare, m...); },
                               schema.schema);

      while (p.peek() != '}')
      {
        // std::string attr_name;
        // if ((err = p.fill(attr_name))) return err;
        // if ((err = p.eat(':'))) return err;

        // bool found = false;
        // for (int i = 0; i < n_members; i++)
        // {
        //   if (attr_name == A[i].name)
        //   {
        //     if (A[i].filled)
        //       return make_json_error("Duplicate json key: ", attr_name);
            
        //     if ((err = A[i].parse_value(p))) return err;
        //     A[i].filled = true;
        //     found = true;
        //     break;
        //   }
        // }

        // bool found = false;
        // if ((err = p.eat('"'))) return err;
        // for (int i = 0; i < n_members; i++)
        // {
        //   int len = A[i].name_len;
        //   if (!strncmp(&p.peek(), A[i].name, len))
        //   {
        //     for (int k = 0; k < len; k++)
        //       p.skip_one();
        //     if ((err = p.eat('"', false))) return err;
        //     if ((err = p.eat(':'))) return err;
        //     if (A[i].filled)
        //       return make_json_error("Duplicate json key: ", A[i].name);
            
        //     if ((err = A[i].parse_value(p))) return err;
        //     A[i].filled = true;
        //     found = true;
        //     break;
        //   }
        // }

        bool found = false;
        if ((err = p.eat('"'))) return err;
        char symbol[50];
        int symbol_size = 0;
        while (!p.eof() and p.peek() != '"' and symbol_size < 50)
          symbol[symbol_size++] = p.get();
        symbol[symbol_size] = 0;
        if ((err = p.eat('"', false))) return err;
  
        for (int i = 0; i < n_members; i++)
        {
          int len = A[i].name_len;
          if (!strncmp(symbol, A[i].name, len))
          {
            if ((err = p.eat(':'))) return err;
            if (A[i].filled)
              return make_json_error("Duplicate json key: ", A[i].name);
            
            if ((err = A[i].parse_value(p))) return err;
            A[i].filled = true;
            found = true;
            break;
          }
        }
        
        if (!found)
          //return make_json_error("Unexpected json key: ", attr_name);
          return make_json_error("Unknown json key");
        p.eat_spaces();
        if (p.peek() == ',')
        {
          if ((err = p.eat(','))) return err;
        }
      }
      if ((err = p.eat('}'))) return err;

      return json_ok;
    }

    template <typename C, typename O, typename S>
    json_error json_decode(C& input, O& obj, S schema)
    {
      auto stream = decode_stringstream(input);
      json_parser<decode_stringstream> p(stream);
      return json_decode2(p, obj, schema);
    }
    
  }

}
