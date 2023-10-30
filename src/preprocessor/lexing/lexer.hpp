#ifndef PREPROCESSOR_LEXER_HPP
#define PREPROCESSOR_LEXER_HPP

#include "../../common/common.hpp"

#include <string>
#include <vector>
#include <unordered_set>

PREPROCESSOR_API_BEGIN

class lexer {
private:
    std::unordered_set<std::string> m_preprocessed;
    std::vector<std::string>        m_buffer;

public:
    lexer();

    [[nodiscard]]
    auto process_multi_line_comment(std::fstream& file) -> bool;

    [[nodiscard]]
    auto copy_file_contents(const std::string& path) -> std::string;

    [[nodiscard]]
    auto process_include(const std::string& line) -> bool;

    auto preprocess(const std::string& path) -> void;
};

PREPROCESSOR_API_END

#endif // !PREPROCESSOR_LEXER_HPP
