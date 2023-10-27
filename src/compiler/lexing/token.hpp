
#include <string.h>
#ifndef LEXER_TOKEN_HPP

#include <cstddef>
#include <string>
#include <optional>

#include "../../common.hpp"

#include "token_type.hpp"

COMPILER_API_BEGIN

struct span {
    std::size_t begin{ 0 }, end{ 0 };
};

class source_location {
private:
    std::string m_file;
    std::size_t m_line, m_column;
public:
    inline explicit source_location(std::string file, std::size_t line, std::size_t col) noexcept
        : m_file(file), m_line(line), m_column(col)
    {}

    inline const std::string& source_file() const noexcept {
        return m_file;
    }

    inline std::size_t line() const noexcept {
        return m_line;
    }

    inline std::size_t column() const noexcept {
        return m_column;
    }

    static inline source_location from(std::string file, std::size_t line, std::size_t col) noexcept {
        return source_location(file, line, col);
    }
};

class token {
private:
    token_type m_type;
    span m_span;
    source_location m_location;
    std::optional<std::string> m_lexeme;
public:
    token() = delete;

    inline
        token(token_type type,
            span span,
            source_location location,
            std::optional<std::string> lexeme = std::nullopt)
        : m_type(type), m_span(span), m_location(location), m_lexeme(lexeme)
    {}

    inline token_type type() const noexcept { return m_type; }
    inline const span& span() const noexcept { return m_span; }
    inline const source_location& source_location() const noexcept { return m_location; }

    inline const std::optional<std::string>& lexeme() const noexcept { return m_lexeme; }
    inline std::optional<std::string>& lexeme() noexcept { return m_lexeme; }
};

COMPILER_API_END

#define LEXER_TOKEN_HPP
#endif // !LEXER_TOKEN_HPP