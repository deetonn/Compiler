
#include <random>
#ifndef _PARSER_PROD_ASSIGNMENT_H

#include <optional>
#include <memory>

#include "../../../common/common.hpp"
#include "../../types.hpp"

#include "node.hpp"

#include "../visitor.hpp"

COMPILER_API_BEGIN

// Assignments are expressions in C.
// With them as expressions, you can do stuff like:
// call_function(data = call_function());
// The expression would evaluate to "data" after the assignment.

class assignment : public expression {
private:
    identifier m_assignee;
    std::optional<std::unique_ptr<expression>> m_expr;
    source_location m_location;
public:
    COMPILER_API inline assignment(
        const identifier& assignee,
        const source_location& location,
        std::optional<std::unique_ptr<expression>> expr = std::nullopt
    ) noexcept 
        : m_location(location)
    {
        m_assignee = assignee;
        m_expr = std::move(expr);
    }

    virtual COMPILER_API void accept(ast_visitor& vis)  {
        return vis.visit_assignment(*this);
    }
    virtual COMPILER_API const source_location& location() const noexcept override {
        return m_location;
    }

    COMPILER_API inline const identifier& assignee() const noexcept {
        return m_assignee;
    }
    
    COMPILER_API inline const std::optional<std::unique_ptr<expression>>& expression() const noexcept {
        return m_expr;
    }

    COMPILER_API inline bool has_expression() const noexcept {
        return m_expr.has_value();
    }
};

COMPILER_API_END

#define _PARSER_PROD_ASSIGNMENT_H
#endif // !_PARSER_PROD_ASSIGNMENT_H