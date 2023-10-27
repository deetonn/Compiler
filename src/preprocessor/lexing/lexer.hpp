#ifndef PREPROCESSOR_LEXER_HPP
#define PREPROCESSOR_LEXER_HPP

#include "../../common/common.hpp"

#include <string>

PREPROCESSOR_API_BEGIN

[[nodiscard]]
auto copy_file_contents(const std::string& path) -> std::string;

[[nodiscard]]
auto process_include(const std::string& line) -> bool;

auto preprocess(const std::string& path) -> void;

PREPROCESSOR_API_END

#endif // !PREPROCESSOR_LEXER_HPP
