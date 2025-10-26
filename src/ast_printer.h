#pragma once

#include "ast.h"
#include <initializer_list>

namespace lox {
class AstPrinter : public ExprVisitor
{
 public:
    std::string Print(Expr* expr);
    Object Visit(Binary* expr) override;
    Object Visit(Grouping* expr) override;
    Object Visit(Literal* expr) override;
    Object Visit(Unary* expr) override;
 private:
    std::string Parenthesize(const std::string& name, std::initializer_list<Expr*> exprs);
};
}