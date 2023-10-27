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

#include "../../common.hpp"
#include "constants.hpp"

#include "token.hpp"
#include "../../include/result.hpp"
#include "../../include/error.hpp"

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

    inline auto lex_single_char(char c) -> result<token, error> {
        switch (c) {
        case left_paren:
            return make_token(token_type::LEFT_PAREN);
        case right_paren:
            return make_token(token_type::RIGHT_PAREN);
        case left_brace:
            return make_token(token_type::LEFT_BRACE);
        case right_brace:
            return make_token(token_type::RIGHT_BRACE);
        case comma:
            return make_token(token_type::COMMA);
        case dot:
            return make_token(token_type::DOT);
        case minus:
            if (peek_next() == greater_than) {
                this->move_forward();
                return make_token(token_type::ARROW);
            }
            if (peek_next() == equal) {
                this->move_forward();
                return make_token(token_type::PLUS_EQUAL);
            }
            return make_token(token_type::MINUS);
        case plus:
            if (peek_next() == equal) {
                this->move_forward();
                return make_token(token_type::PLUS_EQUAL);
            }
            return make_token(token_type::ADD);
        case slash:
            if (peek_next() == equal) {
                this->move_forward();
                return make_token(token_type::SLASH_EQUAL);
            }
            return make_token(token_type::SLASH);
        case star:
            if (peek_next() == equal) {
                this->move_forward();
                return make_token(token_type::STAR_EQUAL);
            }
            return make_token(token_type::STAR);
        case lesser_than:
            if (peek_next() == equal) {
                this->move_forward();
                return make_token(token_type::LESSER_EQUALS);
            }
            return make_token(token_type::LESSER_THAN);
        case greater_than:
            if (peek_next() == equal) {
                this->move_forward();
                return make_token(token_type::GREATER_EQUALS);
            }
            return make_token(token_type::GREATER_THAN);
        case eof:
            return make_token(token_type::END_OF_FILE);
        }

        return error("unexpected character ({})", c);
    }

    inline void move_forward() noexcept {
        m_internals.position++;
        m_internals.column++;
        m_span.end++;
    }

    inline auto peek_next() const noexcept -> char {
        auto& src = m_source_info.contents();
        if (m_internals.position + 1 >= src.size()) {
            return eof;
        }
        return src.at(m_internals.position + 1);
    }

    inline auto make_token(token_type kind, bool use_source = false) noexcept -> token {
        m_span.begin = m_span.end;
        if (use_source) {
            std::string contents = this->get_current_contents();
            return token(kind, m_span, get_source_location(), contents);
        }
        else {
            return token(kind, m_span, get_source_location());
        }
    }

    inline auto get_source_location() noexcept -> source_location {
        return source_location::from(m_source_info.file_name(),
             m_internals.line,
             m_internals.column
        );
    }

    inline auto get_current_contents() const noexcept -> std::string {
        auto& contents = m_source_info.contents();
        auto start_pos = m_span.begin;
        auto count = m_span.end - m_span.begin;
        return contents.substr(start_pos, count);
    }
};

COMPILER_API_END

#endif // !COMPILER_LEXER_HPP
