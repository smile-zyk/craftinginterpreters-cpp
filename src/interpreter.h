#pragma once


#include "ast.h"
#include "environment.h"
#include "token.h"

namespace lox
{
class Interpreter : public expr::ExprVisitor, stmt::StmtVisitor
{
  public:
    void Interpret(const Program &program);
    
    Object Visit(expr::Binary *expr) override;
    Object Visit(expr::Grouping *expr) override;
    Object Visit(expr::Literal *expr) override;
    Object Visit(expr::Unary *expr) override;
    Object Visit(expr::Variable* expr) override;
    Object Visit(expr::Assign* expr) override;

    Object Visit(stmt::Expression *stmt) override;
    Object Visit(stmt::Print *stmt) override;
    Object Visit(stmt::Var* stmt) override;

  private:
    bool IsTruthy(const Object &obj);
    bool IsEqual(const Object &left, const Object &right);
    void CheckNumberOperands(const Token &oper, std::initializer_list<Object> objs);

    Object Evaluate(expr::Expr *expr);
    void Execute(stmt::Stmt *stmt);

  private:
    Environment environment_;
};
} // namespace lox