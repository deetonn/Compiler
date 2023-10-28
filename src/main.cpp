#include "preprocessor/lexing/lexer.hpp"
#include "compiler/lexing/lexer.hpp"
#include <iostream>
#include <format>

// link this in?
constexpr const char name[] = "Compiler";
constexpr const char version[] = "0.0.1";

#define FAIL(fmt, ...) std::cerr << std::format("{} v{}", name, version) << "\n\n";\
  std::cerr << std::format(fmt, ##__VA_ARGS__) << '\n'; return -1

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
    // NOTE: If you are to preprocess, do it here.
    //       "src" is a std::string with the file contents.
    auto lexer = compiler::lexer{src};

    auto lex_result = lexer.lex_tokens();

    if (lex_result.is_err()) {
        FAIL("lexer failed. ({})", lex_result.get_err()->what());
    }

    auto tokens = lexer.release_tokens();

    for (auto& token : tokens) {
        println("{}", token.to_string());
    }

    return 0;
}
