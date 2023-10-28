
#ifndef COMPILER_COMMON_HPP

#include <cstdlib>
#include <iostream>

#define COMPILER_API_BEGIN namespace compiler {
#define COMPILER_API_END }

#define PREPROCESSOR_API_BEGIN namespace preprocessor {
#define PREPROCESSOR_API_END }

#define COMPILER_API
#define NODISCARD [[nodiscard]]
#define DISCARD(expr) ((void)expr)

#define TODO(msg) std::cerr << "TODO: " msg << '\n'; std::exit(-1)

#define COMPILER_COMMON_HPP
#endif // !COMPILER_COMMON_HPP