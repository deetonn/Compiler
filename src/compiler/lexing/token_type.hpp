
#include <corecrt_math.h>
#ifndef LEXER_TOKEN_TYPE_HPP

#include "../../common.hpp"

#include <string>

COMPILER_API_BEGIN

// tokens (C23) (https://en.cppreference.com/w/c/keyword)

enum class token_type {
    LEFT_BRACE,
    RIGHT_BRACE,

    LEFT_PAREN,
    RIGHT_PAREN,

    COMMA,
    DOT,
    // The "->" operator.
    ARROW,
    AMPERSAND,

    EQUALS,
    EQUALS_EQUALS,
    NOT_EQUAL,
    // (or exclaimation point)
    BANG,
    BITWISE_OR,
    OR,
    BITWISE_AND,
    AND,
    DOUBLE_QUOTE,
    SINGLE_QUOTE,
    GREATER_THAN,
    GREATER_EQUALS,
    LESSER_THAN,
    LESSER_EQUALS,
    BITWISE_NOT,
    SEMI_COLON,
    // keywords
  
    // https://en.cppreference.com/w/c/language/_Alignas
    ALIGNAS,
    // https://en.cppreference.com/w/c/language/_Alignof
    ALIGNOF,
    // no one uses this useless keyword, we will accept it
    // but it ignore it.
    AUTO,
    // https://en.cppreference.com/w/c/language/arithmetic_types#Boolean_type
    BOOL,
    // https://en.cppreference.com/w/c/language/break
    BREAK,
    // https://en.cppreference.com/w/c/language/switch
    CASE,
    // https://en.cppreference.com/w/c/language/arithmetic_types#Character_types
    CHAR,
    // https://en.cppreference.com/w/c/language/const
    CONST,
    // https://en.cppreference.com/w/c/language/continue
    CONTINUE,
    // https://en.cppreference.com/w/c/language/switch
    DEFAULT,
    // https://en.cppreference.com/w/c/language/do
    DO,
    // https://en.cppreference.com/w/c/language/types
    DOUBLE,
    // https://en.cppreference.com/w/c/language/types
    LONG_DOUBLE,
    // https://en.cppreference.com/w/c/language/if
    ELSE,
    // https://en.cppreference.com/w/c/language/enum
    ENUM,
    // https://en.cppreference.com/w/c/language/storage_duration
    EXTERN,
    // Boolean literals
    TRUE, FALSE,
    // https://en.cppreference.com/w/c/language/types
    FLOAT,
    // https://en.cppreference.com/w/c/keyword/for
    FOR,
    // https://en.cppreference.com/w/c/keyword/goto
    GOTO,
    // https://en.cppreference.com/w/c/keyword/if
    IF,
    // https://en.cppreference.com/w/c/keyword/inline
    INLINE,
    // https://en.cppreference.com/w/c/keyword/int
    INT,
    // https://en.cppreference.com/w/c/keyword/long
    LONG,
    // https://en.cppreference.com/w/c/keyword/nullptr
    NULLPTR,
    // https://en.cppreference.com/w/c/keyword/register
    REGISTER,
    // https://en.cppreference.com/w/c/keyword/restrict
    RESTRICT,
   // https://en.cppreference.com/w/c/keyword/return
    RETURN,
    // https://en.cppreference.com/w/c/keyword/short
    SHORT,
    // https://en.cppreference.com/w/c/keyword/signed
    SIGNED,
    // https://en.cppreference.com/w/c/keyword/sizeof
    SIZEOF,
    // https://en.cppreference.com/w/c/keyword/static
    STATIC,
    // https://en.cppreference.com/w/c/keyword/static_assert
    STATIC_ASSERT,
    // https://en.cppreference.com/w/c/keyword/struct
    STRUCT,
    // https://en.cppreference.com/w/c/keyword/switch
    SWITCH,
    // https://en.cppreference.com/w/c/keyword/thread_local
    THREAD_LOCAL,
    // https://en.cppreference.com/w/c/keyword/typedef
    TYPEDEF,
    // https://en.cppreference.com/w/c/keyword/typeof
    TYPEOF,
    // https://en.cppreference.com/w/c/keyword/typeof_unqual
    TYPEOF_UNQUAL,
    // https://en.cppreference.com/w/c/keyword/union
    UNION,
    // https://en.cppreference.com/w/c/keyword/unsigned
    UNSIGNED,
    // https://en.cppreference.com/w/c/keyword/void
    VOID,
    // https://en.cppreference.com/w/c/keyword/volatile
    VOLATILE,
    // https://en.cppreference.com/w/c/keyword/while
    WHILE,
};

// NOTE: there are still some keywords that I haven't added
// because they are prefixed with "_". I generally assume that means
// it has some niche functionality which I don't care about yet.

COMPILER_API_END

#define LEXER_TOKEN_TYPE_HPP
#endif // !LEXER_TOKEN_TYPE_HPP