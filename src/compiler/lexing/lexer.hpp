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

    inline auto lex_tokens() noexcept -> result<void, error> {
        while (m_internals.position <= m_source_info.contents().size()) {
            auto lex_result = this->lex_single_char(this->peek_current());
            if (lex_result.is_err()) {
                // TODO: add an actual diagnostics system for outputting errors.
                const char* msg = lex_result.get_err()->what();
                eprintln("[LEXER]: failed to lex contents. ({})", msg);
                return error("{}", msg);
            }
            else {
                auto&& token = std::move(*lex_result.get());
                // discard empty tokens.
                if (token.type() != token_type::EMPTY) {
                    m_tokens.push_back(token);
                }
            }
        }

        return {};
    }

    inline auto release_tokens() noexcept -> std::vector<token> {
        return std::move(m_tokens);
    }
     
    inline auto lex_single_char(char c) -> result<token, error> {
        if (is_valid_identifier_char(c)) {
            return this->lex_identifier();
        }

        if (is_valid_number_start(c)) {
            return this->lex_numeric_literal();
        }

        switch (c) {
        case '\n':
            m_internals.line += 1;
            m_internals.column = 0;
            move_forward();
            return make_token(token_type::EMPTY);
        case '\r':
            if (peek_next() == '\n') {
                move_forward();
                m_internals.line += 1;
                m_internals.column = 0;
            }
            return make_token(token_type::EMPTY);
        case semi_colon:
            return make_token(token_type::SEMI_COLON);
        case ' ':
            return make_token(token_type::EMPTY);
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

        return error("unexpected character ({}) at ({})", c, get_source_location().to_string());
    }

    inline auto lex_numeric_literal() -> result<token, error> {
        auto contents = std::string{};

        // TODO: Integrals can contain postfixes like "i" or "u" to infer the type. 
        //       Handle these cases.

        char next;
        bool encountered_dot = false;
        while (is_valid_number_content(next = peek_current())) {
            if (next == '.') {
                if (encountered_dot) {
                    return error("invalid numeric literal. floating point numbers can only contain one \".\"");
                }
                encountered_dot = true;
            }
            contents.push_back(next);
            move_forward();
        }

        if (encountered_dot) {
            return make_token(token_type::FLOATING_POINT_LITERAL, true);
        }
        else {
            return make_token(token_type::INTEGER_LITERAL, true);
        }
    }

    inline auto lex_identifier() -> result<token, error> {
        auto contents = std::string{};

        char next;
        while (true) {
            next = peek_current();
            contents.push_back(next);

            if (!is_valid_identifier_char(peek_next())) {
                break;
            }

            move_forward();
        }

        if (keywords.contains(contents)) {
            auto tt = keywords.at(contents);
            return make_token(tt);
        }

        return make_token(token_type::IDENTIFIER, true);
    }

    inline void move_forward() noexcept {
        m_internals.position++;
        m_internals.column++;
        m_span.end++;
    }

    inline auto peek_current() const noexcept -> char {
        auto& src = m_source_info.contents();
        if (m_internals.position >= src.size()) {
            return eof;
        }
        return src.at(m_internals.position);
    }

    inline auto peek_next() const noexcept -> char {
        auto& src = m_source_info.contents();
        if (m_internals.position + 1 >= src.size()) {
            return eof;
        }
        return src.at(m_internals.position + 1);
    }

    //inline auto make_token_with_explicit_lexeme(token_type kind, std::string&& lexeme) noexcept -> token {

    //}

    inline auto make_token(token_type kind, bool use_source = false) noexcept -> token {
        if (use_source) {
            std::string contents = this->get_current_contents();
            m_internals.position += contents.size();
            m_span.begin = m_span.end;
            return token(kind, m_span, get_source_location(), contents);
        }
        else {
            m_internals.position += 1;
            m_span.begin = m_span.end;
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
