
#include <float.h>
#ifndef _COMPILER_TYPES_HPP

#include "../common/common.hpp"
#include "lexing/token_type.hpp"

#include "../common/error.hpp"
#include "../common/result.hpp"

#include <optional>
#include <string>
#include <format>
#include <cstddef>
#include <bitset>

COMPILER_API_BEGIN

// A span that represents one point to another.
struct source_span {
    std::size_t begin{ 0 }, end{ 0 };
};

// A source-location. A description of where in the source code something occured.
class source_location {
private:
    std::string m_file;
    std::size_t m_line, m_column;
public:
    inline explicit source_location(std::string file, std::size_t line, std::size_t col) noexcept
        : m_file(file), m_line(line), m_column(col)
    {}

    // The name of the source file.
    inline const std::string& source_file() const noexcept {
        return m_file;
    }
    // The line at while the token occurs at in the source file.
    inline std::size_t line() const noexcept {
        return m_line;
    }
    // The column at while the token occurs at. (this is relative to the line)
    inline std::size_t column() const noexcept {
        return m_column;
    }

    inline std::string to_string() const noexcept {
        return std::format("{}:{}:{}", source_file(), line(), column());
    }

    // static constructor.
    static inline source_location from(std::string file, std::size_t line, std::size_t col) noexcept {
        return source_location(file, line, col);
    }
};

// A token formed from lexical analysis.
class token {
private:
    token_type m_type;
    source_span m_span;
    source_location m_location;
    std::optional<std::string> m_lexeme;
public:
    token() = delete;

    inline
    token(token_type type,
          source_span span,
          source_location location,
          std::optional<std::string> lexeme = std::nullopt
    ) : m_type(type), m_span(span), m_location(location), m_lexeme(lexeme)
    {}

    // the type of this token.
    inline token_type type() const noexcept { return m_type; }
    // the span at which this token occurs at.
    inline const source_span& span() const noexcept { return m_span; }
    // this tokens source location.
    inline const source_location& source_location() const noexcept { return m_location; }

    // The optional contents attached to this token. This is set when the type() is an identifier, string or number.
    inline const std::optional<std::string>& lexeme() const noexcept { return m_lexeme; }
    inline std::optional<std::string>& lexeme() noexcept { return m_lexeme; }

    inline std::string to_string() const noexcept {
        bool has_content = lexeme().has_value();
        if (has_content) {
            auto& content = lexeme().value();
            return std::format("Token({}) [{}] at ({})",
                token_type_to_string(type()),
                content,
                source_location().to_string());
        }
        return std::format("Token({}) at ({})", token_type_to_string(type()), source_location().to_string());
    }
};

// The amount of possible modifiers are:
// 1. const
// 2. unsigned
// 3. signed
// 4. volatile
// 5. static
// 6. extern
static constexpr size_t FLAG_COUNT = 7;

static constexpr size_t FLAG_CONST = 0;
static constexpr size_t FLAG_UNSIGNED = 1;
static constexpr size_t FLAG_SIGNED = 2;
static constexpr size_t FLAG_VOLATILE = 3;
static constexpr size_t FLAG_STATIC = 4;
static constexpr size_t FLAG_EXTERN = 5;
static constexpr size_t FLAG_POINTER = 6;

enum type_modifier {
  mod_const,
  mod_unsigned,
  mod_signed,
  mod_volatile,
  mod_static,
  mod_extern,
  mod_pointer,
  mod_count  
};

class type_information {
private:
    std::string m_name;
    std::bitset<mod_count> m_flags;
public:
    inline explicit type_information(const std::string& name, auto... flags) noexcept {
        m_name = name;
        (m_flags.set(flags, true), ...);
    }

    inline auto has_valid_modifiers() const noexcept -> result<void, error> {
        if (m_flags.test(mod_unsigned) && m_flags.test(mod_signed)) {
            return error("(signed/unsigned mismatch) type cannot be signed & unsigned");
        }
        if (m_flags.test(mod_unsigned) && !is_integral()) {
            return error("(unsigned non-integral) non-integral type cannot be unsigned.");
        }
        if (m_flags.test(mod_signed) && !is_integral()) {
            return error("(signed non-integral) non-integral type cannot be signed.");
        }

        return {};
    }

    inline auto is_integral() const noexcept -> bool {
        if (m_name == "int") return true;
        if (m_name == "long") return true;
        if (m_name == "long long") return true;
        if (m_name == "short") return true;
        if (m_name == "float") return true;
        if (m_name == "char") return true;
        return false;
    }
};

COMPILER_API_END

#define _COMPILER_TYPES_HPP
#endif // !_COMPILER_TYPES_HPP