
#ifndef _PARSER_PROD_NODE_HPP

#include "../../../common/common.hpp"
#include "../../types.hpp"
#include "../../../common/io.hpp"

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
class expression : public ast_node {
public:
    virtual ~expression() = default;
    virtual void accept(ast_visitor& visitor) {}

    virtual const source_location& location() const noexcept {
        eprintln("ERROR: you called node.location() on a node without location() overriden");
        std::exit(-1);
        return (source_location&)(*(source_location*)0);
    }
};
// The base class of all AST nodes that represent a statement.
class statement : public ast_node {
public:
    virtual ~statement() = default;
    virtual void accept(ast_visitor& visitor) {}

    virtual const source_location& location() const noexcept {
        eprintln("ERROR: you called node.location() on a node without location() overriden.");
        std::exit(-1);
        return (source_location&)(*(source_location*)0);
    }
};
// The base class of all AST nodes that represent a declaration.
class declaration : public ast_node {
public:
    virtual ~declaration() = default;
    virtual void accept(ast_visitor& visitor) {}

    virtual const source_location& location() const noexcept {
        eprintln("ERROR: you called node.location() on a node without location() overriden.");
        std::exit(-1);
        // just to stop not-returning errors. (obviously std::exit stops this problem.)
        return (source_location&)(*(source_location*)0);
    }
};

COMPILER_API_END

#define _PARSER_PROD_NODE_HPP
#endif // !_PARSER_PROD_NODE_HPP