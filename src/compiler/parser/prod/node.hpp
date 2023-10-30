
#ifndef _PARSER_PROD_NODE_HPP

#include "../../../common/common.hpp"
#include "../../types.hpp"

class ast_visitor;

COMPILER_API_BEGIN

// The very base class of all AST nodes.
class ast_node {
public:
    virtual ~ast_node() = default;
    virtual void accept(ast_visitor& visitor) = 0;

    virtual const source_location& location() const noexcept = 0;
};

// The base class of all AST nodes that represent an expression.
class expression : public ast_node {};
// The base class of all AST nodes that represent a statement.
class statement : public ast_node {};
// The base class of all AST nodes that represent a declaration.
class declaration : public ast_node {};

COMPILER_API_END

#define _PARSER_PROD_NODE_HPP
#endif // !_PARSER_PROD_NODE_HPP