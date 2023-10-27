
#include <unordered_map>
#ifndef COMPILER_LEXING_CONSTANTS_HPP

#include "../../common.hpp"

#include "token_type.hpp"

#include <string>

COMPILER_API_BEGIN

static const std::unordered_map<std::string, token_type> keywords = {
      { "alignas", token_type::ALIGNAS },
      { "alignof", token_type::ALIGNOF },
      { "auto", token_type::AUTO },
      { "bool", token_type::BOOL },
      { "break", token_type::BREAK },
      { "case", token_type::CASE },
      { "char", token_type::CHAR },
      { "const", token_type::CONST },
      { "continue", token_type::CONTINUE },
      { "default", token_type::DEFAULT },
      { "do", token_type::DO },
      { "double", token_type::DOUBLE },
      { "else", token_type::ELSE },
      { "enum", token_type::ENUM },
      { "extern", token_type::EXTERN },
      { "false", token_type::FALSE },
      { "float", token_type::FLOAT },
      { "for", token_type::FOR },
      { "goto", token_type::GOTO },
      { "if", token_type::IF },
      { "inline", token_type::INLINE },
      { "int", token_type::INT },
      { "long", token_type::LONG },
      { "nullptr", token_type::NULLPTR },
      { "register", token_type::REGISTER },
      { "restrict", token_type::RESTRICT },
      { "return", token_type::RETURN },
      { "short", token_type::SHORT },
      { "signed", token_type::SIGNED },
      { "sizeof", token_type::SIZEOF },
      { "static", token_type::STATIC },
      { "static_assert", token_type::STATIC_ASSERT },
      { "struct", token_type::STRUCT },
      { "switch", token_type::SWITCH },
      { "thread_local", token_type::THREAD_LOCAL },
      { "true", token_type::TRUE },
      { "typedef", token_type::TYPEDEF },
      { "typeof", token_type::TYPEOF },
      { "typeof_unqual", token_type::TYPEOF_UNQUAL },
      { "union", token_type::UNION },
      { "unsigned", token_type::UNSIGNED },
      { "void", token_type::VOID },
      { "volatile", token_type::VOLATILE },
      { "while", token_type::WHILE }
};

#define CONSTANT_CHAR(identifier, value) static constexpr inline char identifier = value

CONSTANT_CHAR(left_brace, '{');
CONSTANT_CHAR(right_brace, '}');

CONSTANT_CHAR(left_paren, '(');
CONSTANT_CHAR(right_paren, ')');

CONSTANT_CHAR(comma, ',');
CONSTANT_CHAR(dot, '.');

CONSTANT_CHAR(equal, '=');

CONSTANT_CHAR(plus, '+');
CONSTANT_CHAR(minus, '-');
CONSTANT_CHAR(star, '*');
CONSTANT_CHAR(slash, '/');

CONSTANT_CHAR(greater_than, '>');
CONSTANT_CHAR(lesser_than, '<');

CONSTANT_CHAR(eof, '\0');

COMPILER_API_END // !COMPILER_API_BEGIN

#define COMPILER_LEXING_CONSTANTS_HPP
#endif // !COMPILER_LEXING_CONSTANTS_HPP