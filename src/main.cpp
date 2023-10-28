#include "preprocessor/lexing/lexer.hpp"
#include "compiler/lexing/lexer.hpp"
#include <iostream>
#include <format>

#define FAIL(fmt, ...) std::cerr << std::format(fmt, ##__VA_ARGS__) << '\n'; return -1

int main(int argc, char** argv) {
    // NOTE: argv[0] is always the path of this executable.
    if (argc < 2) {
        FAIL("expected at least one argument. (the source file)");
    }
    const auto source_file = argv[1];
    auto source_info = compiler::source_info::from_name(source_file);

    if (source_info.is_err()) {
        FAIL("invalid source file provided. ({})", source_info.get_err()->what());
    }

    auto src = *source_info.get();
    auto lexer = compiler::lexer{src};

    return 0;
}
