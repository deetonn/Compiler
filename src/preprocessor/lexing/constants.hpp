#ifndef PREPROCESSOR_LEXING_CONSTANTS_HPP
#define PREPROCESSOR_LEXING_CONSTANTS_HPP

#include "../../common/common.hpp"
#include "token_type.hpp"

#include <unordered_map>

PREPROCESSOR_API_BEGIN

// C preprocessor tokens
// TODO: some tokens may be missing, and some may not be part of the standard
static constexpr std::unordered_map<std::string, token_type> tokens = {
        // standard preprocessor operators
      { "#",                    token_type::HASHTAG },

        // standard preprocessor directives
      { "include",              token_type::INCLUDE },
      { "define",               token_type::DEFINE },
      { "if",                   token_type::IF },
      { "elif",                 token_type::ELIF },
      { "else",                 token_type::ELSE },
      { "endif",                token_type::ENDIF },
      { "ifdef",                token_type::IFDEF },
      { "ifndef",               token_type::IFNDEF },
      { "undef",                token_type::UNDEF },
      { "pragma",               token_type::PRAGMA },
      { "using",                token_type::USING },
      { "error",                token_type::ERROR },
      { "warning",              token_type::WARNING },
      { "elifdef",              token_type::ELIFDEF },
      { "elifndef",             token_type::ELIFNDEF },
      { "embed",                token_type::EMBED },

        // standard predefined identifier
      { "__func__",             token_type::FUNC },

        // standard predefined macros
      { "__DATE__",             token_type::DATE },
      { "__TIME__",             token_type::TIME },
      { "__FILE__",             token_type::FILE },
      { "__LINE__",             token_type::LINE },
      { "__STDC__",             token_type::STDC },
      { "__STDC_HOSTED__",      token_type::STDC_HOSTED },
      { "__STDC_NO_ATOMICS__",  token_type::STDC_NO_ATOMICS },
      { "__STDC_NO_COMPLEX__",  token_type::STDC_NO_COMPLEX },
      { "__STDC_NO_THREADS__",  token_type::STDC_NO_THREADS },
      { "__STDC_NO_VLA__",      token_type::STDC_NO_VLA },
      { "__STDC_VERSION__",     token_type::STDC_VERSION },

        // comments
      { "//",                   token_type::SINGLE_LINE_COMMENT },
      { "/*",                   token_type::MULTI_LINE_COMMENT_ENTRY },
      { "*/",                   token_type::MULTI_LINE_COMMENT_EXIT }
};

PREPROCESSOR_API_END

#endif // !PREPROCESSOR_LEXING_CONSTANTS_HPP
