#pragma once

#include <initializer_list>

#include "ast.h"
#include "token.h"

namespace lox
{
class Interpreter : public ExprVisitor
{
  public:
    void Interpret(Expr* expr);    
    Object Visit(Binary *expr) override;
    Object Visit(Grouping *expr) override;
    Object Visit(Literal *expr) override;
    Object Visit(Unary *expr) override;

  private:
    bool IsTruthy(const Object& obj);    
    bool IsEqual(const Object& left, const Object& right);
    void CheckNumberOperands(const Token& oper, std::initializer_list<Object> objs);

    Object Evaluate(Expr *expr);
};
} // namespace lox