#pragma once

#include "ast.h"

namespace lox
{
class AstPrinter : public expr::ExprVisitor
{
  public:
    std::string Print(expr::Expr *expr);
    Object Visit(expr::Binary *expr) override;
    Object Visit(expr::Grouping *expr) override;
    Object Visit(expr::Literal *expr) override;
    Object Visit(expr::Unary *expr) override;
    Object Visit(expr::Variable *expr) override;
    Object Visit(expr::Assign *expr) override;

  private:
    std::string Parenthesize(const std::string &name, std::initializer_list<expr::Expr *> exprs);
};
} // namespace lox