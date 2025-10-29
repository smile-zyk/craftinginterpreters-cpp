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
using ExprList = std::vector<ExprUniquePtr>;
using StmtList = std::vector<StmtUniquePtr>;
using Program = std::vector<StmtUniquePtr>;

namespace expr
{
class Binary;
class Grouping;
class Literal;
class Logical;
class Unary;
class Variable;
class Assign;
class Call;

class ExprVisitor
{
  public:
    virtual Object Visit(Binary *expr) = 0;
    virtual Object Visit(Grouping *expr) = 0;
    virtual Object Visit(Literal *expr) = 0;
    virtual Object Visit(Unary *expr) = 0;
    virtual Object Visit(Variable *expr) = 0;
    virtual Object Visit(Assign *expr) = 0;
    virtual Object Visit(Logical *expr) = 0;
    virtual Object Visit(Call *expr) = 0;
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

class Logical : public Expr
{
  public:
    Logical(ExprUniquePtr left, const Token &oper, ExprUniquePtr right)
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

class Call : public Expr
{
  public:
    Call(ExprUniquePtr callee, const Token &paren, ExprList arguments)
        : callee_(std::move(callee)), paren_(paren), arguments_(std::move(arguments))
    {
    }

    Object Accept(ExprVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    Expr *callee()
    {
        return callee_.get();
    }

    const Token &paren()
    {
        return paren_;
    }

    const ExprList &arguments()
    {
        return arguments_;
    }

  private:
    ExprUniquePtr callee_;
    Token paren_;
    ExprList arguments_;
};
} // namespace expr

namespace stmt
{
class Expression;
class Print;
class Var;
class Block;
class If;
class While;
class Function;
class Return;

class StmtVisitor
{
  public:
    virtual Object Visit(Expression *stmt) = 0;
    virtual Object Visit(Print *stmt) = 0;
    virtual Object Visit(Var *stmt) = 0;
    virtual Object Visit(Block *stmt) = 0;
    virtual Object Visit(If *stmt) = 0;
    virtual Object Visit(While *stmt) = 0;
    virtual Object Visit(Function *stmt) = 0;
    virtual Object Visit(Return *stmt) = 0;
};

class Stmt
{
  public:
    virtual Object Accept(StmtVisitor *visitor) = 0;
};

class Expression : public Stmt
{
  public:
    Expression(ExprUniquePtr expression) : expression_(std::move(expression)) {}

    Object Accept(StmtVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    expr::Expr *expression()
    {
        return expression_.get();
    }

  private:
    ExprUniquePtr expression_;
};

class Print : public Stmt
{
  public:
    Print(ExprUniquePtr expression) : expression_(std::move(expression)) {}

    Object Accept(StmtVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    expr::Expr *expression()
    {
        return expression_.get();
    }

  private:
    ExprUniquePtr expression_;
};

class Var : public Stmt
{
  public:
    Var(const Token &name, ExprUniquePtr initializer) : name_(name), initializer_(std::move(initializer)) {}

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
    ExprUniquePtr initializer_;
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

class If : public Stmt
{
  public:
    If(ExprUniquePtr condition, StmtUniquePtr then_branch, StmtUniquePtr else_branch)
        : condition_(std::move(condition)), then_branch_(std::move(then_branch)), else_branch_(std::move(else_branch))
    {
    }

    Object Accept(StmtVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    expr::Expr *condition()
    {
        return condition_.get();
    }

    Stmt *then_branch()
    {
        return then_branch_.get();
    }

    Stmt *else_branch()
    {
        return else_branch_.get();
    }

  private:
    ExprUniquePtr condition_;
    StmtUniquePtr then_branch_;
    StmtUniquePtr else_branch_;
};

class While : public Stmt
{
  public:
    While(ExprUniquePtr condition, StmtUniquePtr body) : condition_(std::move(condition)), body_(std::move(body)) {}

    Object Accept(StmtVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    expr::Expr *condition()
    {
        return condition_.get();
    }

    Stmt *body()
    {
        return body_.get();
    }

  private:
    ExprUniquePtr condition_;
    StmtUniquePtr body_;
};

class Function : public Stmt
{
  public:
    Function(const Token &name, const std::vector<Token> &params, StmtUniquePtr body)
        : name_(name), params_(params), body_(std::move(body))
    {
    }

    Object Accept(StmtVisitor *visitor) override
    {
        return visitor->Visit(this);
    }

    const Token &name()
    {
        return name_;
    }

    const std::vector<Token> &params()
    {
        return params_;
    }

    Stmt* body()
    {
        return body_.get();
    }

  private:
    Token name_;
    std::vector<Token> params_;
    StmtUniquePtr body_;
};

class Return : public Stmt
{
  public:
    Return(const Token& keyword, ExprUniquePtr value): keyword_(keyword), value_(std::move(value)){}

    Object Accept(StmtVisitor *visitor) override
    {
        return visitor->Visit(this);
    }
    
    const Token& keyword() { return keyword_; }
    
    expr::Expr* value() { return value_.get(); }

  private:
    Token keyword_;
    ExprUniquePtr value_;
};

} // namespace stmt
} // namespace lox