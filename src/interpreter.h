#pragma once
#include <memory>

#include "ast.h"
#include "environment.h"
#include "token.h"

namespace lox
{
class Interpreter : public expr::ExprVisitor, stmt::StmtVisitor
{
  public:
    Interpreter();
    void Interpret(const Program &program);

    Object Visit(expr::Binary *expr) override;
    Object Visit(expr::Grouping *expr) override;
    Object Visit(expr::Literal *expr) override;
    Object Visit(expr::Unary *expr) override;
    Object Visit(expr::Variable *expr) override;
    Object Visit(expr::Assign *expr) override;
    Object Visit(expr::Logical *expr) override;
    Object Visit(expr::Call* expr) override;

    Object Visit(stmt::Expression *stmt) override;
    Object Visit(stmt::Print *stmt) override;
    Object Visit(stmt::Var *stmt) override;
    Object Visit(stmt::Block *stmt) override;
    Object Visit(stmt::If *stmt) override;
    Object Visit(stmt::While *stmt) override;
    Object Visit(stmt::Function *stmt) override;
    Object Visit(stmt::Return *strm) override;

    void Execute(stmt::Stmt *stmt);
    void ExecuteBlock(const StmtList &statements, Environment* environment);

    Environment* globals() { return globals_.get(); }

  private:
    bool IsTruthy(const Object &obj);
    bool IsEqual(const Object &left, const Object &right);
    void CheckNumberOperands(const Token &oper, std::initializer_list<Object> objs);

    Object Evaluate(expr::Expr *expr);
  private:
    std::unique_ptr<Environment> globals_;
    Environment* environment_;
};
} // namespace lox