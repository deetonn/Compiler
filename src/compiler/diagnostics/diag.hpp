
#ifndef _COMPILER_DIAGNOSTICS_DIAG_HPP

#include "../../common/common.hpp"
#include "../types.hpp"

#include <string>
#include <vector>

COMPILER_API_BEGIN

enum class diag_level {
    warning, error  
};

class diagnostic {
private:
    std::string m_message;
    source_location m_location;
    diag_level m_level;
    std::vector<std::string> m_notes{};
public:
    COMPILER_API diagnostic() = delete;
    COMPILER_API diagnostic(
        const std::string& message,
        const source_location& location,
        diag_level level,
        // sadly a copy...
        std::optional<std::vector<std::string>> notes = std::nullopt
    ) noexcept;

    COMPILER_API inline const std::string& message() const noexcept { return m_message; }
    COMPILER_API inline const source_location& location() const noexcept { return m_location; }

    COMPILER_API inline diag_level level() const noexcept { return m_level; }

    COMPILER_API inline void push_note(const std::string& note) noexcept { m_notes.push_back(note); }
    COMPILER_API std::string build_into_message(const std::vector<std::string>& src) const noexcept;
};

class diagnostic_builder {
private:
    std::string m_message{"no message provided"};
    source_location m_location{source_location::invalid()};
    diag_level m_level{diag_level::error};
    std::vector<std::string> m_notes{};
public:
    COMPILER_API inline diagnostic_builder() noexcept = default;
    COMPILER_API inline diagnostic_builder(const diagnostic_builder&) noexcept = default;
    COMPILER_API inline diagnostic_builder(diagnostic_builder&&) noexcept = default;

    COMPILER_API inline diagnostic_builder& with_message(const std::string& message) noexcept {
        m_message = message;
        return *this;
    }

    COMPILER_API inline diagnostic_builder& with_location(const source_location& location) noexcept {
        m_location = location;
        return *this;
    }

    COMPILER_API inline diagnostic_builder& with_level(diag_level level) noexcept {
        m_level = level;
        return *this;
    }

    COMPILER_API inline diagnostic_builder& with_note(const std::string& note) noexcept {
        m_notes.push_back(note);
        return *this;
    }

    COMPILER_API inline diagnostic build() const noexcept {
        return diagnostic(m_message, m_location, m_level, std::optional(m_notes));
    }
};

COMPILER_API inline diagnostic_builder make_diag_builder() noexcept {
    return diagnostic_builder();
}

COMPILER_API_END

#define _COMPILER_DIAGNOSTICS_DIAG_HPP
#endif // !_COMPILER_DIAGNOSTICS_DIAG_HPP