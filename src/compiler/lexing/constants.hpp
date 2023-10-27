
#include <unordered_map>
#ifndef COMPILER_LEXING_CONSTANTS_HPP

#include "../../common.hpp"

#include "token_type.hpp"

#include <string>

COMPILER_API_BEGIN

static const std::unordered_map<std::string, TokenType> keywords = {
      { "alignas", TokenType::Alignas },
      { "alignof", TokenType::Alignof },
      { "auto", TokenType::Auto },
      { "bool", TokenType::Bool },
      { "break", TokenType::Break },
      { "case", TokenType::Case },
      { "char", TokenType::Char },
      { "const", TokenType::Const },
      { "continue", TokenType::Continue },
      { "default", TokenType::Default },
      { "do", TokenType::Do },
      { "double", TokenType::Double },
      { "else", TokenType::Else },
      { "enum", TokenType::Enum },
      { "extern", TokenType::Extern },
      { "false", TokenType::False },
      { "float", TokenType::Float },
      { "for", TokenType::For },
      { "goto", TokenType::Goto },
      { "if", TokenType::If },
      { "inline", TokenType::Inline },
      { "int", TokenType::Int },
      { "long", TokenType::Long },
      { "nullptr", TokenType::Nullptr },
      { "register", TokenType::Register },
      { "restrict", TokenType::Restrict },
      { "return", TokenType::Return },
      { "short", TokenType::Short },
      { "signed", TokenType::Signed },
      { "sizeof", TokenType::Sizeof },
      { "static", TokenType::Static },
      { "static_assert", TokenType::StaticAssert },
      { "struct", TokenType::Struct },
      { "switch", TokenType::Switch },
      { "thread_local", TokenType::ThreadLocal },
      { "true", TokenType::True },
      { "typedef", TokenType::Typedef },
      { "typeof", TokenType::Typeof },
      { "typeof_unqual", TokenType::TypeofUnqual },
      { "union", TokenType::Union },
      { "unsigned", TokenType::Unsigned },
      { "void", TokenType::Void },
      { "volatile", TokenType::Volatile },
      { "while", TokenType::While }
};

#define CONSTANT_CHAR(identifier, value) static constexpr inline char identifier = value

CONSTANT_CHAR(left_brace, '{');
CONSTANT_CHAR(right_brace, '}'); 

COMPILER_API_END // !COMPILER_API_BEGIN

#define COMPILER_LEXING_CONSTANTS_HPP
#endif // !COMPILER_LEXING_CONSTANTS_HPP