#pragma once

#include <memory>
#include <vector>

#include "object.h"
#include "token.h"

namespace lox
{
namespace expr
{
class Expr;
}

namespace stmt
{
class Stmt;
}

using ExprUniquePtr = std::unique_ptr<expr::Expr>;
using StmtUniquePtr = std::unique_ptr<stmt::Stmt>;
using Program = std::vector<StmtUniquePtr>;
using StmtList = std::vector<StmtUniquePtr>;

namespace expr
{
class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;
class Assign;

class ExprVisitor
{
  public:
    virtual Object Visit(Binary *expr) = 0;
    virtual Object Visit(Grouping *expr) = 0;
    virtual Object Visit(Literal *expr) = 0;
    virtual Object Visit(Unary *expr) = 0;
    virtual Object Visit(Variable *expr) = 0;
    virtual Object Visit(Assign *expr) = 0;
};

class Expr
{
  public:
    virtual Object Accept(ExprVisitor *visitor) = 0;
};

class Binary : public Expr
{
  public:
    Binary(ExprUniquePtr left, const Token &oper, ExprUniquePtr right)
        : left_(std::move(left)), operator_(oper), right_(std::move(right))
    {
    }

    Object Accept(ExprVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    Expr *left()
    {
        return left_.get();
    }
    const Token &oper()
    {
        return operator_;
    }
    Expr *right()
    {
        return right_.get();
    }

  private:
    ExprUniquePtr left_;
    Token operator_;
    ExprUniquePtr right_;
};

class Grouping : public Expr
{
  public:
    Grouping(ExprUniquePtr expression) : expression_(std::move(expression)) {}

    Object Accept(ExprVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    Expr *expression()
    {
        return expression_.get();
    }

  private:
    ExprUniquePtr expression_;
};

class Literal : public Expr
{
  public:
    Literal(Object value) : value_(value) {}

    Object Accept(ExprVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    const Object &value()
    {
        return value_;
    }

  private:
    Object value_;
};

class Unary : public Expr
{
  public:
    Unary(const Token &oper, ExprUniquePtr right) : operator_(oper), right_(std::move(right)) {}

    Object Accept(ExprVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    const Token &oper()
    {
        return operator_;
    }
    Expr *right()
    {
        return right_.get();
    }

  private:
    Token operator_;
    ExprUniquePtr right_;
};

class Variable : public Expr
{
  public:
    Variable(const Token &name) : name_(name) {}

    Object Accept(ExprVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    const Token &name()
    {
        return name_;
    }

  private:
    Token name_;
};

class Assign : public Expr
{
  public:
    Assign(const Token &name, ExprUniquePtr value) : name_(name), value_(std::move(value)) {}

    Object Accept(ExprVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    Expr *value()
    {
        return value_.get();
    }

    const Token &name()
    {
        return name_;
    }

  private:
    Token name_;
    ExprUniquePtr value_;
};

} // namespace expr

namespace stmt
{
class Expression;
class Print;
class Var;
class Block;

class StmtVisitor
{
  public:
    virtual Object Visit(Expression *stmt) = 0;
    virtual Object Visit(Print *stmt) = 0;
    virtual Object Visit(Var *stmt) = 0;
    virtual Object Visit(Block *stmt) = 0;
};

class Stmt
{
  public:
    virtual Object Accept(StmtVisitor *visitor) = 0;
};

class Expression : public Stmt
{
  public:
    Expression(std::unique_ptr<expr::Expr> expression) : expression_(std::move(expression)) {}

    Object Accept(StmtVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    expr::Expr *expression()
    {
        return expression_.get();
    }

  private:
    std::unique_ptr<expr::Expr> expression_;
};

class Print : public Stmt
{
  public:
    Print(std::unique_ptr<expr::Expr> expression) : expression_(std::move(expression)) {}

    Object Accept(StmtVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    expr::Expr *expression()
    {
        return expression_.get();
    }

  private:
    std::unique_ptr<expr::Expr> expression_;
};

class Var : public Stmt
{
  public:
    Var(const Token &name, std::unique_ptr<expr::Expr> initializer) : name_(name), initializer_(std::move(initializer))
    {
    }

    Object Accept(StmtVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    expr::Expr *initializer()
    {
        return initializer_.get();
    }

    const Token &name()
    {
        return name_;
    }

  private:
    Token name_;
    std::unique_ptr<expr::Expr> initializer_;
};

class Block : public Stmt
{
  public:
    Block(StmtList statements) : statements_(std::move(statements)) {}
    Object Accept(StmtVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    const StmtList &statements()
    {
        return statements_;
    }

  private:
    StmtList statements_;
};

} // namespace stmt
} // namespace lox