#include "lexing/lexer.hpp"
#include <iostream>

int main() {
    constexpr auto x = test_func(10, 2);
    std::cout << x << '\n';

    return 0;
}
