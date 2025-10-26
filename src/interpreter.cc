#include "interpreter.h"

#include <iostream>

#include "error.h"
#include "lox.h"

using namespace lox;

void Interpreter::Interpret(Expr *expression)
{
    try
    {
        Object value = Evaluate(expression);
        std::cout << ObjectToString(value) << std::endl;
    }
    catch (const RuntimeError &e)
    {
        Lox::Error(e);
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
    if (IsObjectInstance<nullptr_t>(a) && IsObjectInstance<nullptr_t>(b))
    {
        return true;
    }
    if (IsObjectInstance<nullptr_t>(a) || IsObjectInstance<nullptr_t>(b))
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
