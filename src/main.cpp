#include "preprocessor/lexing/lexer.hpp"
#include "compiler/lexing/lexer.hpp"
#include <iostream>

int main() {
    const auto x = preprocessor::test_func();
    if (!x.has_value())
        std::cout << "std::nullopt\n";

    const auto y = compiler::test_func(10, 2);
    std::cout << y << '\n';

    return 0;
}
