
#ifndef _PARSER_PROD_NODE_HPP

#include "../../../common/common.hpp"
#include "../../types.hpp"
#include "../visitor.hpp"

COMPILER_API_BEGIN

class ast_node {
public:
    virtual ~ast_node() = default;
    virtual void accept(ast_visitor& visitor) = 0;

    virtual const source_location& location() const noexcept = 0;
};

COMPILER_API_END

#define _PARSER_PROD_NODE_HPP
#endif // !_PARSER_PROD_NODE_HPP