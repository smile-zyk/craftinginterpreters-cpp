#pragma once

#include <functional>
#include <vector>
#include "interpreter.h"
#include "object.h"

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
    BuiltinCallable(const std::string& func_name, CallFunc func, int arity) : func_name_(func_name), func_(func), arity_(arity) {}

    Object Call(Interpreter *interpreter, const std::vector<Object>& arguments) override
    {
        return func_(interpreter, arguments);
    }

    int arity() override
    {
        return arity_;
    }

    std::string ToString() override
    {
        return "<native func " + func_name_ + ">";
    }

  private:
    std::string func_name_;
    CallFunc func_;
    int arity_;
};

} // namespace lox