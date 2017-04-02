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
