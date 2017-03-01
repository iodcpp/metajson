#pragma once

#include <experimental/string_view>
#include <iod/metamap/metamap.hh>
#include <iod/metajson/utils.hh>

namespace iod
{
  namespace impl
  {

    template <typename... W>
    auto json_error(W... what)
    {
      std::stringstream ss;
      ss << "json_decode error: ";

      auto add = [&ss] (auto w) { ss << w; };
      apply_each(add, what...);
      return std::runtime_error(ss.str());
    }

    struct json_parser
    {
      template <typename S>
      inline json_parser(S&& s) :ss(s) {}

      inline char peek() {
        if (ss.eof())
          throw json_error("Unexpected eof.");

        eat_spaces();
        return ss.peek();
      }

      inline void eat(char c) {
        if (ss.eof())
          throw json_error("Unexpected eof.");

        eat_spaces();
        if (ss.peek() != c)
          throw json_error("Unexpected char. Got '", char(ss.peek()), "' expected ", c);
        else ss.get();
      }

      inline void eat_spaces()
      {
        while (!ss.eof() and ss.peek() < 33) ss.get();
      }

      template <typename T>
      void fill(T& t)
      {
        ss >> t;
      }

      inline void fill(std::string& str)
      {
        eat('"');
        while (true)
        {
          while (!ss.eof() and ss.peek() != '"')
            str.push_back(ss.get());

          // Count the prev backslashes.
          int end = str.size();
          int sb = end - 1;
          while (sb >= 0 and str[sb] == '\\')
            sb--;

          int number_of_backslashes = end - sb - 1;

          // If an even nomber of backslash, the double quote is not escaped.
          if (!(number_of_backslashes % 2)) break;
          else // double quote is escaped, add it to the string and continue.
            str.push_back(ss.get()); // Add the 
        }
        eat('"');
      }
      
      std::stringstream ss;
    };
    
    template <typename O, typename S>
    void json_decode(json_parser& p, O& obj, json_object_<S> schema)
    {
      p.eat('{');
      struct attr_info { bool filled; const char* name; std::function<void(json_parser&)> parse_value; };
      constexpr int n_members = std::tuple_size<decltype(schema.members)>();
      attr_info A[n_members];
      int i = 0;
      auto prepare = [&] (auto m) {
        A[i].filled = false;
        A[i].name = symbol_string(m.name);

        if constexpr(has_key(m, _json_key)) {
            A[i].name = m.json_key;
          }
        
        A[i].parse_value = [m,&obj] (json_parser& p) {
          p.fill(symbol_member_access(obj, m.name));
        };

        i++;
      };

      std::experimental::apply([&] (auto... m) { apply_each(prepare, m...); },
                               schema.members);

      while (p.peek() != '}')
      {
        std::string attr_name;
        p.fill(attr_name);
        p.eat(':');

        bool found = false;
        for (int i = 0; i < n_members; i++)
        {
          if (attr_name == A[i].name)
          {
            if (A[i].filled)
              throw json_error("Duplicate json key: ", attr_name);
            
            A[i].parse_value(p);
            A[i].filled = true;
            found = true;
            break;
          }
        }

        if (!found)
          throw json_error("Unexpected json key: ", attr_name);

        if (p.peek() == ',')
          p.eat(',');
      }
      p.eat('}');
    }

    template <typename C, typename O, typename S>
    void json_decode(C& input, O& obj, json_object_<S> schema)
    {
      json_parser p(input);
      return json_decode(p, obj, schema);
    }
    
  }

}
