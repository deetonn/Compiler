#include "diag.hpp"

#include "../../common/io.hpp"

#include <format>

constexpr static inline auto FAILED_TO_BUILD = "(failed to build diagnostic)";

compiler::diagnostic::diagnostic(const std::string& message, const source_location& location, diag_level level, std::optional<std::vector<std::string>> notes) noexcept
    : m_location(location), m_message(message), m_level(level) 
{
    if (notes.has_value()) {
        m_notes = std::move(notes.value());
    }
}

std::string compiler::diagnostic::build_into_message(const std::vector<std::string>& src) const noexcept {
    // Expect "src" to be a vector with each string representing a line.

    // Get the source location as a string.
    const auto source_info = m_location.to_string();

    if (m_location.line() > src.size()) {
        eprintln("failed to build diagnostic into string!");
        eprintln("the source information's line is larger than the source itself.");
        return FAILED_TO_BUILD;
    }

    const std::string& line_of_diag = src.at(m_location.line());
    const auto prefix = m_level == diag_level::error ? "ERROR" : "WARNING";

    // Returns something like:
    /*
    [WARNING]: this is an error message
      --> (main.c:2:2)
     4 | line of code
       = note: this is a note
    */

    auto baseline_message = std::format("[{}]: {}\n  --> ({})\n {} | {}\n",
        prefix, m_message, source_info, m_location.line(), line_of_diag
    );

    if (m_notes.empty()) {
        return baseline_message;
    }

    for (const auto& note : m_notes) {
        baseline_message += std::format(" = note: {}", note);
    }

    return baseline_message;
}