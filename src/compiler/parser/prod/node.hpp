
#ifndef _PARSER_PROD_NODE_HPP

#include "../../../common/common.hpp"

#include "../visitor.hpp"

COMPILER_API_BEGIN

class ast_node {
public:
    virtual ~ast_node() = default;
    virtual void accept(ast_visitor& visitor) = 0;

    virtual 
};

COMPILER_API_END

#define _PARSER_PROD_NODE_HPP
#endif // !_PARSER_PROD_NODE_HPP