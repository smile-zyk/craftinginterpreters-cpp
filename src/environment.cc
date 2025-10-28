#include "environment.h"
#include "error.h"

namespace lox
{
void Environment::Define(const std::string &name, const Object &value) 
{
    values_.insert_or_assign(name, value);
}

const Object &Environment::Get(const Token &name) 
{
    auto it = values_.find(name.lexeme());
    if(it != values_.end())
    {
        return it->second;
    }
    if(enclosing_ != nullptr) return enclosing_->Get(name);
    throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
}

void Environment::Assign(const Token &name, const Object &value)
{
    auto it = values_.find(name.lexeme());
    if(it != values_.end())
    {
        it->second = value;
        return;
    }

    if(enclosing_ != nullptr)
    {
        enclosing_->Assign(name, value);
        return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
}

} // namespace lox