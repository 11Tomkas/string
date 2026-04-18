#include <iostream>
#include <string>
#include "string.hpp"

int main()
{
    String str1("hello world goodbye world");
    String str2(2, 'a');

    str2.assign(str1, 0, 25);
    std::cout << "str1: " << str1 << '\n';
    std::cout << "str2: " << str2 << '\n';

    return 0;
}