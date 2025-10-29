#include "callable.h"
#include "ast.h"
#include "environment.h"
#include "control_exception.h"

#include <memory>

namespace lox
{

BuiltinCallable::BuiltinCallable(const std::string &func_name, CallFunc func, int arity)
    : func_name_(func_name), func_(func), arity_(arity)
{
}

Object BuiltinCallable::Call(Interpreter *interpreter, const std::vector<Object> &arguments)
{
    return func_(interpreter, arguments);
}

int BuiltinCallable::arity()
{
    return arity_;
}

std::string BuiltinCallable::ToString()
{
    return "<native func " + func_name_ + ">";
}

UserDefineCallable::UserDefineCallable(stmt::Function *declaration) : declaration_(declaration) {}

Object UserDefineCallable::Call(Interpreter *interpreter, const std::vector<Object> &arguments)
{
    auto environment = std::make_unique<Environment>(interpreter->globals());

    for (int i = 0; i < declaration_->params().size(); i++)
    {
        environment->Define(declaration_->params().at(i).lexeme(), arguments.at(i));
    }

    stmt::Block *block = dynamic_cast<stmt::Block *>(declaration_->body());

    if (block == nullptr)
    {
        return nullptr;
    }
    try 
    {
        interpreter->ExecuteBlock(block->statements(), environment.get());
    }
    catch(const control::Return& e)
    {
        return e.value();
    }

    return nullptr;
}

int UserDefineCallable::arity()
{
    return declaration_->params().size();
}

std::string UserDefineCallable::ToString()
{
    return "<fn " + declaration_->name().lexeme() + ">";
}

} // namespace lox