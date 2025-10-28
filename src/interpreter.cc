#include "interpreter.h"

#include <iostream>
#include <memory>
#include <utility>

#include "ast.h"
#include "environment.h"
#include "error.h"
#include "object.h"

using namespace lox;
using namespace lox::expr;
using namespace lox::stmt;

void Interpreter::Interpret(const Program &program)
{
    try
    {
        for (const auto &statement : program)
        {
            Execute(statement.get());
        }
    }
    catch (const RuntimeError &e)
    {
        lox::Error(e);
    }
}

Object Interpreter::Visit(Binary *expr)
{
    Object left = Evaluate(expr->left());
    Object right = Evaluate(expr->right());

    switch (expr->oper().type())
    {
    case Token::Type::kGreater:
        CheckNumberOperands(expr->oper(), {left, right});
        return std::get<double>(left) > std::get<double>(right);
    case Token::Type::kGreaterEqual:
        CheckNumberOperands(expr->oper(), {left, right});
        return std::get<double>(left) >= std::get<double>(right);
    case Token::Type::kLess:
        CheckNumberOperands(expr->oper(), {left, right});
        return std::get<double>(left) < std::get<double>(right);
    case Token::Type::kLessEqual:
        CheckNumberOperands(expr->oper(), {left, right});
        return std::get<double>(left) <= std::get<double>(right);
    case Token::Type::kBangEqual:
        return !IsEqual(left, right);
    case Token::Type::kEqualEqual:
        return IsEqual(left, right);
    case Token::Type::kPlus:
        if (IsObjectInstance<double>(left) && IsObjectInstance<double>(right))
        {
            return std::get<double>(left) + std::get<double>(right);
        }
        if (IsObjectInstance<std::string>(left) && IsObjectInstance<std::string>(right))
        {
            return std::get<std::string>(left) + std::get<std::string>(right);
        }
        throw RuntimeError(expr->oper(), "Operands must be two numbers or two strings.");
    case Token::Type::kMinus:
        CheckNumberOperands(expr->oper(), {left, right});
        return std::get<double>(left) - std::get<double>(right);
    case Token::Type::kSlash:
        CheckNumberOperands(expr->oper(), {left, right});
        return std::get<double>(left) / std::get<double>(right);
    case Token::Type::kStar:
        CheckNumberOperands(expr->oper(), {left, right});
        return std::get<double>(left) * std::get<double>(right);
    default:
        break;
    }
    return nullptr;
}

Object Interpreter::Visit(Grouping *expr)
{
    return Evaluate(expr->expression());
}

Object Interpreter::Visit(Literal *expr)
{
    return expr->value();
}

Object Interpreter::Visit(Unary *expr)
{
    Object right = Evaluate(expr->right());

    switch (expr->oper().type())
    {
    case Token::Type::kBang:
        return IsTruthy(right);
    case Token::Type::kMinus:
        CheckNumberOperands(expr->oper(), {right});
        return std::get<double>(right);
    default:
        break;
    }

    return nullptr;
}

Object Interpreter::Visit(Variable *expr)
{
    return environment_->Get(expr->name());
}

Object Interpreter::Visit(Assign* expr)
{
    Object value = Evaluate(expr->value());
    environment_->Assign(expr->name(), value);
    return value;
}

Object Interpreter::Visit(Expression *stmt)
{
    Evaluate(stmt->expression());
    return nullptr;
}

Object Interpreter::Visit(Print *stmt)
{
    Object value = Evaluate(stmt->expression());
    std::cout << ObjectToString(value) << std::endl;
    return nullptr;
}

Object Interpreter::Visit(Var *stmt)
{
    Object value = nullptr;
    if (stmt->initializer() != nullptr)
    {
        value = Evaluate(stmt->initializer());
    }

    environment_->Define(stmt->name().lexeme(), value);
    return nullptr;
}

Object Interpreter::Visit(Block *stmt)
{
    ExecuteBlock(stmt->statements(), std::make_unique<Environment>(environment_.get()));
    return nullptr;
}

bool Interpreter::IsTruthy(const Object &obj)
{
    if (IsObjectInstance<std::nullptr_t>(obj))
    {
        return false;
    }
    if (IsObjectInstance<bool>(obj))
    {
        return std::get<bool>(obj);
    }
    return true;
}

bool Interpreter::IsEqual(const Object &a, const Object &b)
{
    if (IsObjectInstance<std::nullptr_t>(a) && IsObjectInstance<std::nullptr_t>(b))
    {
        return true;
    }
    if (IsObjectInstance<std::nullptr_t>(a) || IsObjectInstance<std::nullptr_t>(b))
    {
        return false;
    }
    if (IsObjectInstance<std::string>(a) && IsObjectInstance<std::string>(b))
    {
        return std::get<std::string>(a) == std::get<std::string>(b);
    }
    if (IsObjectInstance<double>(a) && IsObjectInstance<double>(b))
    {
        return std::get<double>(a) == std::get<double>(b);
    }
    if (IsObjectInstance<bool>(a) && IsObjectInstance<bool>(b))
    {
        return std::get<bool>(a) == std::get<bool>(b);
    }
    return false;
}

void Interpreter::CheckNumberOperands(const Token &oper, std::initializer_list<Object> objs)
{
    bool res = true;
    for (const auto &obj : objs)
    {
        res &= IsObjectInstance<double>(obj);
    }
    if (res)
    {
        return;
    }

    throw RuntimeError(oper, "Operands must be numbers.");
}

Object Interpreter::Evaluate(Expr *expr)
{
    return expr->Accept(this);
}

void Interpreter::Execute(stmt::Stmt *stmt)
{
    stmt->Accept(this);
}

void Interpreter::ExecuteBlock(const StmtList& statements, EnvironmentUniquePtr environment)
{
    EnvironmentUniquePtr previous = std::move(environment_);
    try 
    {
        environment_ = std::move(environment);

        for(const StmtUniquePtr& statement : statements)
        {
            Execute(statement.get());
        }
    } 
    catch (const ParseError& e) 
    {
        lox::Error(e);
    }
    catch (const RuntimeError& e)
    {
        lox::Error(e);
    }
    catch(...)
    {
        environment_ = std::move(previous);
        throw;
    }

    environment_ = std::move(previous);
}