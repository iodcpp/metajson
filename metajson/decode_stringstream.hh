#pragma once

#include <experimental/string_view>
#include <cmath>
namespace iod
{
  using std::experimental::string_view;
  
  namespace internal
  {
    template <typename I>
    void parse_uint(I* val_, const char* str, const char** end)
    {
      I& val = *val_;
      val = 0;
      int i = 0;
      while (i < 40)
      {
        char c = *str;
        if (c < '0' or c > '9') break;
        val = val * 10 + c - '0';
        str++;
        i++;
      }
      if (end)
        *end = str;
    }
   
    template <typename I>
    void parse_int(I* val, const char* str, const char** end)
    {
      bool neg = false;

      if (str[0] == '-')
      {
        neg = true;
        str++;
      }
      parse_uint(val, str, end);
      if (neg)
        *val = -*val;
    }

    inline unsigned long pow10(unsigned int e)
    {
      unsigned long pows[] = {
        1,
        10,
        100,
        1000,
        10000,
        100000,
        1000000,
        10000000,
        100000000,
        1000000000,
        10000000000,
        100000000000,
        1000000000000,
        10000000000000,
        100000000000000,
        1000000000000000,
        10000000000000000
      };

      if (e < 17)
        return pows[e];
      else
      {
        unsigned long res = 1;
        int i = 0;
        for (;i < e; i++)
          res *= 10;
        return res;
      }
    }
    
    template <typename F>
    void parse_float(F* f, const char* str, const char** end)
    {
      // 1.234e-10
      // [sign][int][decimal_part][exp]

      const char* it = str;
      int integer_part;
      parse_int(&integer_part, it, &it);
      *f = integer_part;
      if (*it == '.')
      {
        it++;
        unsigned int decimal_part;
        const char* dec_end;
        parse_uint(&decimal_part, it, &dec_end);
        
        if (dec_end > it)
          *f += F(decimal_part) / pow10(dec_end - it);

        it = dec_end;
      }

      if (*it == 'e' || *it == 'E')
      {
        it++;
        bool neg = false;
        if (*it == '-')
        {
          neg = true;
          it++;
        }

        unsigned int exp = 0;
        parse_uint(&exp, it, &it);
        if (neg)
          *f = *f / pow10(exp);
        else
          *f = *f * pow10(exp);
      }
      
      if (end)
        *end = it;

    }
    
  }

  class decode_stringstream
  {
  public:

    decode_stringstream(string_view buffer_)
      : pos(0),
        bad_(false),
        buffer(buffer_) {}
    
    bool eof() const { return pos >= buffer.size(); }
    auto& peek() const { return buffer[pos]; }
    int get()        { return buffer[pos++]; }
    int bad() const  { return bad_; }

    template <typename T>
    void operator>>(T& value)
      {
        if constexpr(std::is_floating_point<T>::value) {
            // Decode floating point.
            eat_spaces();
            const char* end = nullptr;
            internal::parse_float(&value, buffer.data() + pos, &end);
            if (end == (buffer.data() + pos))
              bad_ = true;
            pos = end - buffer.data();
          }
        else if constexpr (std::is_integral<T>::value) {
            // Decode integer.
            eat_spaces();
            const char* end = nullptr;
            internal::parse_int(&value, buffer.data() + pos, &end);
            if (end == (buffer.data() + pos))
              bad_ = true;
            pos = end - buffer.data();
          }
        else if constexpr (std::is_same<T, std::string>::value) {
            // Decode UTF8 string.
            json_to_utf8(*this, value);
          }
        else if constexpr (std::is_same<T, string_view>::value) {
            // Decoding to stringview does not decode utf8.
            const char* start = buffer.data() + pos;
            bool escaped = false;
            while (peek() != '\0' and (escaped or peek() != '\"'))
            {
              int nb = 0;
              while (peek() == '\\')
                nb++;

              escaped = nb % 2;
              pos++;
            }

            const char* end = buffer.data() + pos;
            value = string_view(start, end);
          }
      }

  private:

    inline void eat_spaces()
      {
        while (peek() >= 0 and peek() < 33) pos++;
      }
    
    int bad_;
    int pos;
    string_view buffer; //
  };
}
