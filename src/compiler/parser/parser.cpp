#include "parser.hpp"

using std::optional;
using std::reference_wrapper;
using std::ref;

COMPILER_API bool compiler::parser::matches(token_type tok) const noexcept
{
    if (m_pos > m_tokens.size()) {
        return false;
    }

    const auto& token = m_tokens.at(m_pos);
    return token.type() == tok;
}

COMPILER_API optional<reference_wrapper<compiler::token>> compiler::parser::expect(token_type type, diagnostic&& diag) noexcept
{
    if (!matches(type)) {
        push_diagnostic(std::move(diag));
        // TODO: how to handle this???
        // We can't return a reference inside a std::optional.
        return std::nullopt;
    }

    return std::ref(m_tokens.at(m_pos));
}

COMPILER_API void compiler::parser::push_diagnostic(diagnostic&& diag) noexcept
{
    m_diags.push_back(std::move(diag));
}
