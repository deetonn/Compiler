#ifndef LEXER_TOKEN_TYPE_HPP

#include "../../common/common.hpp"

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

    ADD, 
    PLUS_EQUAL,
    MINUS, 
    MINUS_EQUAL,
    SLASH, 
    SLASH_EQUAL,
    STAR,
    STAR_EQUAL,

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
    PLUS_PLUS,
    MINUS_MINUS,
    MODULO,
    BITWISE_XOR,
    XOR_EQUALS,
    END_OF_FILE,
    // represents a nothing token.
    EMPTY,

    IDENTIFIER,
    FLOATING_POINT_LITERAL,
    INTEGER_LITERAL,
    STRING_LITERAL,
    CHARACTER_LITERAL,
    // keywords

    // https://en.cppreference.com/w/c/language/_Alignas
    ALIGNAS,
    // https://en.cppreference.com/w/c/language/_Alignof
    ALIGNOF,
    // no one uses this useless keyword, we will accept it
    // but ignore it.
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

inline std::string token_type_to_string(token_type type) {
    switch (type) {
    case token_type::LEFT_BRACE: return "LEFT_BRACE";
    case token_type::RIGHT_BRACE: return "RIGHT_BRACE";
    case token_type::LEFT_PAREN: return "LEFT_PAREN";
    case token_type::RIGHT_PAREN: return "RIGHT_PAREN";
    case token_type::COMMA: return "COMMA";
    case token_type::DOT: return "DOT";
    case token_type::ARROW: return "ARROW";
    case token_type::AMPERSAND: return "AMPERSAND";
    case token_type::ADD: return "ADD";
    case token_type::PLUS_EQUAL: return "PLUS_EQUAL";
    case token_type::MINUS: return "MINUS";
    case token_type::MINUS_EQUAL: return "MINUS_EQUAL";
    case token_type::SLASH: return "SLASH";
    case token_type::SLASH_EQUAL: return "SLASH_EQUAL";
    case token_type::STAR: return "STAR";
    case token_type::STAR_EQUAL: return "STAR_EQUAL";
    case token_type::EQUALS: return "EQUALS";
    case token_type::EQUALS_EQUALS: return "EQUALS_EQUALS";
    case token_type::NOT_EQUAL: return "NOT_EQUAL";
    case token_type::BANG: return "BANG";
    case token_type::BITWISE_OR: return "BITWISE_OR";
    case token_type::OR: return "OR";
    case token_type::BITWISE_AND: return "BITWISE_AND";
    case token_type::AND: return "AND";
    case token_type::DOUBLE_QUOTE: return "DOUBLE_QUOTE";
    case token_type::SINGLE_QUOTE: return "SINGLE_QUOTE";
    case token_type::GREATER_THAN: return "GREATER_THAN";
    case token_type::GREATER_EQUALS: return "GREATER_EQUALS";
    case token_type::LESSER_THAN: return "LESSER_THAN";
    case token_type::LESSER_EQUALS: return "LESSER_EQUALS";
    case token_type::BITWISE_NOT: return "BITWISE_NOT";
    case token_type::SEMI_COLON: return "SEMI_COLON";
    case token_type::END_OF_FILE: return "END_OF_FILE";
    case token_type::ALIGNAS: return "ALIGNAS";
    case token_type::ALIGNOF: return "ALIGNOF";
    case token_type::AUTO: return "AUTO";
    case token_type::BOOL: return "BOOL";
    case token_type::BREAK: return "BREAK";
    case token_type::CASE: return "CASE";
    case token_type::CHAR: return "CHAR";
    case token_type::CONST: return "CONST";
    case token_type::CONTINUE: return "CONTINUE";
    case token_type::DEFAULT: return "DEFAULT";
    case token_type::DO: return "DO";
    case token_type::DOUBLE: return "DOUBLE";
    case token_type::LONG_DOUBLE: return "LONG_DOUBLE";
    case token_type::ELSE: return "ELSE";
    case token_type::ENUM: return "ENUM";
    case token_type::EXTERN: return "EXTERN";
    case token_type::TRUE: return "TRUE";
    case token_type::FALSE: return "FALSE";
    case token_type::FLOAT: return "FLOAT";
    case token_type::FOR: return "FOR";
    case token_type::GOTO: return "GOTO";
    case token_type::IF: return "IF";
    case token_type::INLINE: return "INLINE";
    case token_type::INT: return "INT";
    case token_type::LONG: return "LONG";
    case token_type::NULLPTR: return "NULLPTR";
    case token_type::REGISTER: return "REGISTER";
    case token_type::RESTRICT: return "RESTRICT";
    case token_type::RETURN: return "RETURN";
    case token_type::SHORT: return "SHORT";
    case token_type::SIGNED: return "SIGNED";
    case token_type::SIZEOF: return "SIZEOF";
    case token_type::STATIC: return "STATIC";
    case token_type::STATIC_ASSERT: return "STATIC_ASSERT";
    case token_type::STRUCT: return "STRUCT";
    case token_type::SWITCH: return "SWITCH";
    case token_type::THREAD_LOCAL: return "THREAD_LOCAL";
    case token_type::TYPEDEF: return "TYPEDEF";
    case token_type::TYPEOF: return "TYPEOF";
    case token_type::TYPEOF_UNQUAL: return "TYPEOF_UNQUAL";
    case token_type::UNION: return "UNION";
    case token_type::UNSIGNED: return "UNSIGNED";
    case token_type::VOID: return "VOID";
    case token_type::VOLATILE: return "VOLATILE";
    case token_type::WHILE: return "WHILE";
    case token_type::EMPTY: return "EMPTY";
    case token_type::IDENTIFIER: return "IDENTIFIER";
    case token_type::FLOATING_POINT_LITERAL: return "FLOAT_LITERAL";
    case token_type::INTEGER_LITERAL: return "INTEGER_LITERAL";
    case token_type::STRING_LITERAL: return "STRING_LITERAL";
    case token_type::CHARACTER_LITERAL: return "CHARACTER_LITERAL";
    case token_type::PLUS_PLUS: return "PLUS_PLUS";
    case token_type::MINUS_MINUS: return "MINUS_MINUS";
    case token_type::MODULO: return "MODULO";
    case token_type::BITWISE_XOR: return "BITWISE_XOR";
    case token_type::XOR_EQUALS: return "XOR_EQUALS";
    }

    return "UNKNOWN";
}

// NOTE: there are still some keywords that I haven't added
// because they are prefixed with "_". I generally assume that means
// it has some niche functionality which I don't care about yet.

COMPILER_API_END

#define LEXER_TOKEN_TYPE_HPP
#endif // !LEXER_TOKEN_TYPE_HPP
