#pragma once

#include <functional>
#include <vector>

#include "interpreter.h"
#include "object.h"
#include "ast.h"

namespace lox
{
class Callable
{
  public:
    virtual Object Call(Interpreter *interpreter, const std::vector<Object>& arguments) = 0;
    virtual std::string ToString() = 0;
    virtual int arity() = 0;
};

using CallFunc = std::function<Object(Interpreter *, std::vector<Object>)>;

class BuiltinCallable : public Callable
{
  public:
    BuiltinCallable(const std::string& func_name, CallFunc func, int arity);

    Object Call(Interpreter *interpreter, const std::vector<Object>& arguments) override;

    int arity() override;

    std::string ToString() override;

  private:
    std::string func_name_;
    CallFunc func_;
    int arity_;
};

class UserDefineCallable : public Callable
{
  public:
    UserDefineCallable(stmt::Function* declaration);

    Object Call(Interpreter *interpreter, const std::vector<Object>& arguments) override;

    int arity() override;

    std::string ToString() override;

  private:
    stmt::Function* declaration_;
};

} // namespace lox