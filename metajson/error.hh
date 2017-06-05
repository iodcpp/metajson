#pragma once

#include <string>

namespace iod
{

  enum JSON_ERROR
  {
    JSON_OK = 0
    //JSON_
  }
    
  struct json_error
  {
    json_error& operator=(const json_error&) = default;
    operator bool() { return code != 0; }
    int code; //const char* what;
    //std::string what;
  };
  
  // json_error make_json_error(const char* what) { return {1, std::string("metajson error: ") + what}; }
  // json_error json_no_error() { return {0, ""}; }

  json_error make_json_error(const char* what) { return {1}; }
  json_error json_no_error() { return {0}; }

  static json_error json_ok = json_no_error();
  
  template <typename... W>
  auto make_json_error(W... what)
  {
    // std::stringstream ss;

    // ss << "metajson error: ";
    // auto add = [&ss] (auto w) { ss << w; };
    // apply_each(add, what...);
    //return json_error{1, ss.str()};
    return json_error{1};
  }

}
