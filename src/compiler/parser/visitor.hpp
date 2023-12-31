
#ifndef _PARSER_VISITOR_HPP

#include "../../common/common.hpp"

COMPILER_API_BEGIN

class assignment;
class assignment_declaration;

// for now, until I know what these should return.
using visitor_result = void;

class ast_visitor {
public:
    virtual visitor_result visit_assignment(assignment& node) = 0;
    virtual visitor_result visit_assignment_declaration(assignment_declaration& node) = 0;
};

COMPILER_API_END

#define _PARSER_VISITOR_HPP
#endif // !_PARSER_VISITOR_HPP