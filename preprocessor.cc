

// #define IOD_SYMBOL(X) IOD_SYMBOL##1(X)

//#define IOD_s::name 1


#define TO_BOOL(x, ...)  x
//#define TO_BOOL()  0


// #define TEST_ 1
// #define TEST_IOD__name 2
// #define TEST_1 3

// #define TEST3(A) "TEST_"#A
// #define TEST2(A) TEST3(A)
// #define TEST(X) TEST2(IOD__name)

// template <int N>
// struct symbol;

// IOD_SYMBOL(X)

// template <>
// struct symbol<1>
// {
//   int name;
// };

// template <>
// struct symbol<s::name, 34553>
// {
//   int name;
// };

// get_member(s::name, obj)
// {
//   symbol<s::name, X>
// }
// int main()
// {
// }

//#define D(x)

#include <iostream>

//#define X 1

int main()
{
  //std::cout << TEST(name) << std::endl;
  std::cout << TO_BOOL(1) << std::endl;
}
