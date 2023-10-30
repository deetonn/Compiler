#ifndef PREPROCESSOR_LEXING_TOKEN_TYPE_HPP
#define PREPROCESSOR_LEXING_TOKEN_TYPE_HPP

#include "../../common/common.hpp"

#include <string>

PREPROCESSOR_API_BEGIN

enum class token_type {
    // standard preprocessor operators
    HASHTAG,

    // standard preprocessor directives
    INCLUDE,
    DEFINE,
    IF,
    ELIF,
    ELSE,
    ENDIF,
    IFDEF,
    IFNDEF,
    UNDEF,
    PRAGMA,
    USING,
    ERROR,
    WARNING,
    ELIFDEF,
    ELIFNDEF,
    EMBED,

    // standard predefined identifier
    FUNC,

    // standard predefined macros
    DATE,
    TIME,
    FILE,
    LINE,
    STDC,
    STDC_HOSTED,
    STDC_NO_ATOMICS,
    STDC_NO_COMPLEX,
    STDC_NO_THREADS,
    STDC_NO_VLA,
    STDC_VERSION,

    // comments
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT_ENTRY,
    MULTI_LINE_COMMENT_EXIT
};

inline std::string token_type_to_string(const token_type type) {
    switch (type) {
        case token_type::HASHTAG:                   return "HASHTAG";
        case token_type::INCLUDE:                   return "INCLUDE";
        case token_type::DEFINE:                    return "DEFINE";
        case token_type::IF:                        return "IF";
        case token_type::ELIF:                      return "ELIF";
        case token_type::ELSE:                      return "ELSE";
        case token_type::ENDIF:                     return "ENDIF";
        case token_type::IFDEF:                     return "IFDEF";
        case token_type::IFNDEF:                    return "IFNDEF";
        case token_type::UNDEF:                     return "UNDEF";
        case token_type::PRAGMA:                    return "PRAGMA";
        case token_type::USING:                     return "USING";
        case token_type::ERROR:                     return "ERROR";
        case token_type::WARNING:                   return "WARNING";
        case token_type::ELIFDEF:                   return "ELIFDEF";
        case token_type::ELIFNDEF:                  return "ELIFNDEF";
        case token_type::EMBED:                     return "EMBED";
        case token_type::FUNC:                      return "FUNC";
        case token_type::DATE:                      return "DATE";
        case token_type::TIME:                      return "TIME";
        case token_type::FILE:                      return "FILE";
        case token_type::LINE:                      return "LINE";
        case token_type::STDC:                      return "STDC";
        case token_type::STDC_HOSTED:               return "STDC_HOSTED";
        case token_type::STDC_NO_ATOMICS:           return "STDC_NO_ATOMICS";
        case token_type::STDC_NO_COMPLEX:           return "STDC_NO_COMPLEX";
        case token_type::STDC_NO_THREADS:           return "STDC_NO_THREADS";
        case token_type::STDC_NO_VLA:               return "STDC_NO_VLA";
        case token_type::STDC_VERSION:              return "STDC_VERSION";
        case token_type::SINGLE_LINE_COMMENT:       return "SINGLE_LINE_COMMENT";
        case token_type::MULTI_LINE_COMMENT_ENTRY:  return "MULTI_LINE_COMMENT_ENTRY";
        case token_type::MULTI_LINE_COMMENT_EXIT:   return "MULTI_LINE_COMMENT_EXIT";

        default:                                return "UNKNOWN";
    }
}

PREPROCESSOR_API_END

#endif // !PREPROCESSOR_LEXING_TOKEN_TYPE_HPP
