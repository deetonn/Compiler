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

#include "../types.hpp"
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

// Source information that holds information about the file and its contents.
struct source_info {
private:
    std::string m_file_name;
    std::string m_file_contents;
public:
    // Initialize with a file name.
    inline explicit source_info(const std::string& file_name)
    {
        m_file_name = file_name;
        auto file_stream = std::ifstream{ file_name };
        std::stringstream ss;
        ss << file_stream.rdbuf();

        m_file_contents = ss.str();
    }

    // The name of the file
    NODISCARD
    inline const std::string& file_name() const noexcept {
        return m_file_name;
    }

    // The contents within the file.
    NODISCARD
    inline const std::string& contents() const noexcept {
        return m_file_contents;
    }

    // Static helper function for use with auto.
    NODISCARD
    inline static  
    auto from_name(const std::string& file) -> result<source_info, error> 
    {
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
    source_span m_span{ 0, 0 };
    source_info m_source_info;
public:
    lexer() = delete;
    inline explicit lexer(source_info info) noexcept
        : m_source_info{ info }
    {}

    // Lexes the source contents, populates an inner vector of tokens.
    // NOTE: see lexer::release_tokens() to get the tokens.
    NODISCARD auto lex_tokens() noexcept -> result<void, error>;
     
    // Handles a single token based on the parameter.
    NODISCARD auto lex_single_char(char c) noexcept -> result<token, error>;
    // Handles any numeric literal including signed, unsigned and floating point numbers
    //  relative to the current position.
    NODISCARD auto lex_numeric_literal() noexcept -> result<token, error>;
    // Handles any identifiers, this includes keywords. This is relative to the current position.
    NODISCARD auto lex_identifier() noexcept -> result<token, error>;
    // Handles any string literal relative to the current position.
    NODISCARD auto lex_string_literal() noexcept -> result<token, error>;
    // lexes any character literal relative to the current position.
    NODISCARD auto lex_char_literal() noexcept -> result<token, error>;

    // Handles escape characters such as '\n', '\0' etc...
    NODISCARD auto lex_escape_character(char c) noexcept -> result<char, error>;

    // Move the lexer forward by one character.
    auto move_forward() noexcept -> void;
    // Peek the current character. If there is nothing where we are, eof is returned.
    NODISCARD auto peek_current() const noexcept -> char;
    // Peek the next character. If there is nothing where we are, eof is returned.
    NODISCARD auto peek_next() const noexcept -> char;
    // Advance and then get that character after advancing. If we cant advance, eof is returned.
    NODISCARD auto advance() noexcept -> char;

    // Make a token based on the current state of the lexer. If use_source is true, the token will
    // have its internal contents as the current span of the source contents.
    NODISCARD auto make_token(token_type kind, bool use_source = false) noexcept -> token;
    // Make a token based on the current state of the lexer, but with explicit contents. 
    NODISCARD auto make_token_with_explicit_contents(token_type kind, std::string&& content) noexcept -> token;

    // Get the current source location.
    NODISCARD auto get_source_location() noexcept -> source_location;
    // Get the current source contents, based from the current span.
    NODISCARD auto get_current_contents() const noexcept -> std::string;

    // NOTE: this is inline because its very small.
    // Get the tokens, they are moved from the lexer, to the caller.
    [[nodiscard("this is a move function, the caller will own the tokens after this call.")]]
    inline auto release_tokens() noexcept -> std::vector<token> {
        return std::move(m_tokens);
    }
};

COMPILER_API_END

#endif // !COMPILER_LEXER_HPP
