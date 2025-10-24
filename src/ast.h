#pragma once

#include "object.h"
#include "token.h"
#include <memory>
#include <utility>

namespace lox
{

class Binary;
class Grouping;
class Literal;
class Unary;

class ExprVisitor
{
  public:
    virtual Object Visit(Binary *binary) = 0;
    virtual Object Visit(Grouping *grouping) = 0;
    virtual Object Visit(Literal *literal) = 0;
    virtual Object Visit(Unary *unary) = 0;
};

class Expr
{
  public:
    virtual Object Accept(ExprVisitor *visitor) = 0;
};

class Binary : public Expr
{
  public:
    Binary(std::unique_ptr<Expr> left, Token oper, std::unique_ptr<Expr> right)
        : left_(std::move(left)), operator_(oper), right_(std::move(right))
    {
    }

    Object Accept(ExprVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    const Expr *left()
    {
        return left_.get();
    }
    const Token oper()
    {
        return operator_;
    }
    const Expr *right()
    {
        return right_.get();
    }

  private:
    std::unique_ptr<Expr> left_;
    Token operator_;
    std::unique_ptr<Expr> right_;
};

class Grouping : public Expr
{
  public:
    Grouping(std::unique_ptr<Expr> expression) : expression_(std::move(expression)) {}

    Object Accept(ExprVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    const Expr *expression()
    {
        return expression_.get();
    }

  private:
    std::unique_ptr<Expr> expression_;
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
    Unary(Token oper, std::unique_ptr<Expr> right) : operator_(oper), right_(std::move(right)) {}

    Object Accept(ExprVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    const Token &oper()
    {
        return operator_;
    }
    const Expr *right()
    {
        return right_.get();
    }

  private:
    Token operator_;
    std::unique_ptr<Expr> right_;
};

} // namespace lox