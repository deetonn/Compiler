
#ifndef _COMPILER_PARSER_PROD_ASSIGNMENT_STMT_HPP

#include "../../../common/common.hpp"

#include "node.hpp"
#include "../visitor.hpp"

#include "../../types.hpp"

COMPILER_API_BEGIN

/*
  This is an actual assignment, something such as:
  "int a = 2;"

  NOTE: I'm not sure if this should be classes as a declaration or not.
*/

class assignment_statement : public statement {
private:
  type_information m_type_info;
  identifier m_identifier;
  source_location m_source_location;
  std::optional<std::unique_ptr<expression>> m_expr;
public:
  COMPILER_API inline explicit assignment_statement(
      const type_information& type,
      const identifier& identifier,
      const source_location& location,
      std::optional<std::unique_ptr<expression>> expr = std::nullopt          
  )
    : m_type_info(type)
    , m_identifier(identifier)
    , m_source_location(location)
  {
      if (expr.has_value()) {
          m_expr = std::move(expr.value());            
      }
  }

  inline virtual void accept(ast_visitor& visitor) noexcept {
      return visitor.visit_assignment_statement(*this);          
  } 

  inline virtual const source_location& location() const noexcept {
      return m_source_location;          
  }

  inline const type_information& type() const noexcept {
      return m_type_info;          
  }

  inline const identifier& identifier() const noexcept {
      return m_identifier;
  }

  inline bool has_expr() const noexcept { return m_expr.has_value(); }
  inline expression* expr() noexcept { return m_expr->get(); }
  inline const expression* expr() const noexcept { return m_expr->get(); }
};

COMPILER_API_END

#define _COMPILER_PARSER_PROD_ASSIGNMENT_STMT_HPP
#endif