#include "parser.hpp"
#include "../types.hpp"

#include "../lexing/token_type.hpp"

#include "../../common/io.hpp"
#include <memory>
#include <optional>

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

COMPILER_API error compiler::parser::push_diagnostic(diagnostic&& diag) noexcept
{
    auto ret = error("{}", diag.message());
    m_diags.push_back(std::move(diag));
    return ret;
}

COMPILER_API void compiler::parser::parse(const std::vector<std::string>& src) noexcept
{
    while (!matches(token_type::END_OF_FILE)) {
        this->parse_next();
    }
    // After parsing has been done, check if there are any diagnostics.

    if (m_diags.empty()) {
        return;
    }

    for (const auto& diagnostic : m_diags) {
        eprintln("{}", diagnostic.build_into_message(src));
    }
}

COMPILER_API void compiler::parser::parse_next() noexcept 
{
    if (seq_looks_like_typename()) {
        
    }
}

template<class T>
bool is_any_of(T left, auto... right) noexcept {
    return ((left == right) || ...);
}

COMPILER_API bool compiler::parser::seq_looks_like_typename() const noexcept
{
    const auto maybe_next_token = this->peek();

    if (!maybe_next_token.has_value()) {
        return false;
    }

    using tt = token_type;
    const auto& token = maybe_next_token.value().get();

    if (is_any_of(token.type(),
        tt::SIGNED, tt::UNSIGNED, tt::CHAR,
        tt::SHORT, tt::INT, tt::LONG, 
        tt::VOLATILE, tt::STATIC, tt::REGISTER,
        tt::VOID))
    {
        return true;
    }

    if (is_any_of(token.type(), tt::STRUCT)) {
        return true;
    }

    if (is_any_of(token.type(), tt::IDENTIFIER)) {
        return true;
    }

    if (is_any_of(token.type(), tt::ENUM)) {
        return true;
    }

    return false;
} 

COMPILER_API optional<reference_wrapper<const compiler::token>> compiler::parser::peek() const noexcept {
    if (m_tokens.size() > m_pos) {
        return std::nullopt;
    }

    return std::cref(m_tokens.at(m_pos));
} 

COMPILER_API optional<reference_wrapper<const compiler::token>> compiler::parser::peek_next() const noexcept {
    if (m_tokens.size() > (m_pos + 1)) {
        return std::nullopt;
    }

    return std::cref(m_tokens.at(m_pos + 1));
}

#define PARSE_FAILURE(diagnostic)         \
    auto diag = diagnostic;               \
    auto e = error("{}", diag.message()); \
    push_diagnostic(std::move(diag));     \
    return e;

COMPILER_API result<compiler::type_information, error> compiler::parser::parse_typename() noexcept {
    auto modifiers = std::bitset<type_modifier::mod_count>{};
    std::optional<std::reference_wrapper<const token>> next;
    using tt = token_type;

    while ((next = peek())) {
        switch (next.value().get().type()) {
        case tt::UNSIGNED:
            modifiers.set(mod_unsigned);
            break;
        case tt::SIGNED:
            modifiers.set(mod_signed);
            break;
        case tt::EXTERN:
            modifiers.set(mod_extern);
            break;
        case tt::VOLATILE:
            modifiers.set(mod_volatile);
            break;
        case tt::INT:
            if (modifiers.test(mod_short)) {
                modifiers.set(mod_short, false);
                modifiers.set(mod_short_int, true);        
            }
            else if (modifiers.test(mod_long)) {
                if (modifiers.test(mod_long_int) || modifiers.test(mod_long_long_int)) {
                    PARSE_FAILURE(make_diag_builder()
                        .with_level(diag_level::error)
                        .with_location(next.value().get().location())
                        .with_message("invalid type specifiers")
                        .with_note("got `int` after `long int` or `long long int` was already specified.")
                        .build());
                }
                else if (modifiers.test(mod_long_long)) {
                    modifiers.set(mod_long_long, false);
                    modifiers.set(mod_long_long_int, true);        
                }
                else if (modifiers.test(mod_long)) {
                    modifiers.set(mod_long, false);
                    modifiers.set(mod_long_int);        
                }
                break;
            }
            else if (modifiers.test(mod_int)) {
                PARSE_FAILURE(make_diag_builder()
                    .with_level(diag_level::error)
                    .with_location(next.value().get().location())
                    .with_message("invalid type modifiers. (cannot have `int int`)")
                    .build());        
            }
            else {
                modifiers.set(mod_int, true);        
            }
            break;
        case tt::SHORT:
            if (modifiers.test(mod_short)) {
                PARSE_FAILURE(make_diag_builder()
                    .with_level(diag_level::error)
                    .with_location(next.value().get().location())
                    .with_message("invalid type modifiers. (cannot have `short short`)")
                    .build());
            }
            modifiers.set(mod_short);
            break;
        case tt::LONG:
            if (modifiers.test(mod_long)) {                    
                if (!modifiers.test(mod_long_long)) {
                    modifiers.set(mod_long, false);
                    modifiers.set(mod_long_long, true);
                    break;  
                }
                PARSE_FAILURE(make_diag_builder()
                    .with_level(diag_level::error)
                    .with_location(next.value().get().location())
                    .with_message("invalid type modifiers. (cannot have `long long long`)")
                    .build());
            }
            else if (!modifiers.test(mod_long_long)) {
                modifiers.set(mod_long);        
            }
            else {
                PARSE_FAILURE(make_diag_builder()
                    .with_level(diag_level::error)
                    .with_location(next.value().get().location())
                    .with_message("invalid type specifiers. (cannot have `long long long`)")
                    .build());   
            }
            break;
        }
    }

    // TODO: handle multiple pointers.
    // NOTE: Something like `char** argv`
    if (matches(tt::STAR)) {
        modifiers.set(mod_pointer);
    }

    return type_information("integral", type_kind::integral, std::move(modifiers));
}