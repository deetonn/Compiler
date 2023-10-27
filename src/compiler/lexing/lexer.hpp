#ifndef COMPILER_LEXER_HPP
#define COMPILER_LEXER_HPP

#include <cmath>
#include <cstddef>
#include <string>
#include <optional>
#include <vector>

#include "../../common.hpp"

COMPILER_API_BEGIN

struct span {
  std::size_t begin{0}, end{0};
};

struct _Lexer_internals {
  std::size_t line, column, position;
};

class lexer {
private:
  
};

COMPILER_API_END

#endif // !COMPILER_LEXER_HPP
