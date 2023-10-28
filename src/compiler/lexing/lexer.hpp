#ifndef COMPILER_LEXER_HPP
#define COMPILER_LEXER_HPP

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <optional>
#include <vector>
#include <fstream>
#include <random>

#include "../../common/common.hpp"
#include "constants.hpp"

#include "token.hpp"
#include "../../common/result.hpp"
#include "../../common/error.hpp"
#include "../../common/io.hpp"

#define LEXER_DEBUG 1

COMPILER_API_BEGIN

// Simple class to represent which line, column and position (in the array of source contents)
// that the lexer is at.
struct _Lexer_internals {
    std::size_t line, column, position;
    std::string_view file;
};

struct source_info {
private:
    std::string m_file_name;
    std::string m_file_contents;
public:
    inline explicit source_info(const std::string& file_name)
    {
        m_file_name = file_name;
        auto file_stream = std::ifstream{ file_name };
        std::stringstream ss;
        ss << file_stream.rdbuf();

        m_file_contents = ss.str();
    }

    inline const std::string& file_name() const noexcept {
        return m_file_name;
    }

    inline const std::string& contents() const noexcept {
        return m_file_contents;
    }

    inline static auto from_name(const std::string& file) -> result<source_info, error> {
        try {
            return source_info(file);
        }
        catch (const std::exception& ex) {
            return error("ERROR: failed to read source file. ({})", ex.what());
        }
    }
};

// Simple C lexer.
class lexer {
private:
    // The tokens that have been processed so far.
    std::vector<token> m_tokens{};
    // internal state of the lexer.
    _Lexer_internals m_internals{ 1, 0, 0, "<unknown>" };
    // the current span of the source contents that we're at.
    // m_span.end should always be equal to m_internals.position.
    span m_span{ 0, 0 };
    source_info m_source_info;
public:
    lexer() = delete;
    inline explicit lexer(source_info info) noexcept
        : m_source_info{ info }
    {}

    auto lex_tokens() noexcept -> result<void, error>;
     
    auto lex_single_char(char c) noexcept -> result<token, error>;
    auto lex_numeric_literal() noexcept -> result<token, error>;
    auto lex_identifier() noexcept -> result<token, error>;
    auto lex_string_literal() noexcept -> result<token, error>;
    auto lex_char_literal() noexcept -> result<token, error>;

    auto lex_escape_character(char c) noexcept -> result<char, error>;

    auto move_forward() noexcept -> void;
    auto peek_current() const noexcept -> char;
    auto peek_next() const noexcept -> char;
    auto advance() noexcept -> char;

    auto make_token(token_type kind, bool use_source = false) noexcept -> token;
    auto make_token_with_explicit_contents(token_type kind, std::string&& content) noexcept -> token;

    auto get_source_location() noexcept -> source_location;
    auto get_current_contents() const noexcept -> std::string;

    // NOTE: this is inline because its very small.
    inline auto release_tokens() noexcept -> std::vector<token> {
        return std::move(m_tokens);
    }
};

COMPILER_API_END

#endif // !COMPILER_LEXER_HPP
