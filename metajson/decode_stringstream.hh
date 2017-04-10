#include <experimental/string_view>

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

    // int parse_float(const char* str, const char** end)
    // {
    //   // 1e-10
    // }
    
  }

  class decode_stringstream
  {
  public:

    decode_stringstream(string_view buffer_)
      : cur(buffer_.data()),
        bad_(false),
        buffer(buffer_) {}
    
    inline bool eof() const { return cur >= &buffer.back(); }
    inline auto& peek() const { return *cur; }
    inline int get()        { return *(cur++); }
    inline int bad() const  { return bad_; }

    template <typename T>
    void operator>>(T& value)
      {
        eat_spaces();
        if constexpr(std::is_floating_point<T>::value) {
            // Decode floating point.
          }
        else if constexpr (std::is_integral<T>::value) {
            // Decode integer.
            const char* end = nullptr;
            internal::parse_int(&value, cur, &end);
            if (end == cur)
              bad_ = true;
            cur = end;
          }
        else if constexpr (std::is_same<T, std::string>::value) {
            // Decode UTF8 string.
            json_to_utf8(*this, value);
          }
        else if constexpr (std::is_same<T, string_view>::value) {
            // Decoding to stringview does not decode utf8.
          }
      }

  private:

    inline void eat_spaces()
      {
        while (peek() < 33) ++cur;
      }
    
    int bad_;
    const char* cur;
    string_view buffer; //
  };

}
