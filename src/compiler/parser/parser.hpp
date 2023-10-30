
#include <optional>
#ifndef _COMPILER_PARSER_HPP

#include "../../common/common.hpp"
#include "../../common/result.hpp"
#include "../../common/error.hpp"

#include "../types.hpp"
#include "prod/node.hpp"

#include "prod/assignment.hpp"
#include "prod/node.hpp"

#include "../types.hpp"
#include "../lexing/token_type.hpp"
#include "../diagnostics/diag.hpp"

#include <vector>
#include <memory>

COMPILER_API_BEGIN

using ast = std::vector<std::unique_ptr<ast_node>>;
using token_list = std::vector<token>;

class parser {
private:
    ast m_ast{};
    token_list m_tokens;
    size_t m_pos{0};
    std::vector<diagnostic> m_diags;
public:
    COMPILER_API parser() = delete;
    COMPILER_API parser(token_list&& tokens) noexcept
        : m_tokens(std::move(tokens))
    {}

    COMPILER_API void parse(const std::vector<std::string>& src) noexcept;
    COMPILER_API void parse_next() noexcept;  

    COMPILER_API result<type_information, error> parse_typename() noexcept;

private:
    // check if the current token is of type "tok"
    COMPILER_API bool matches(token_type tok) const noexcept;
    COMPILER_API std::optional<std::reference_wrapper<token>> expect(token_type type, diagnostic&& diag) noexcept;

    COMPILER_API bool seq_looks_like_typename() const noexcept;

    COMPILER_API std::optional<std::reference_wrapper<const token>> peek_next() const noexcept;
    COMPILER_API std::optional<std::reference_wrapper<const token>> peek() const noexcept;

    COMPILER_API error push_diagnostic(diagnostic&& diag) noexcept;
};

COMPILER_API_END

#define _COMPILER_PARSER_HPP
#endif