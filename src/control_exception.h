#pragma once

#include <exception>

#include "object.h"
#include "token.h"

namespace lox
{
namespace control
{
class Return : public std::exception
{
  public:
    Return(const Token &keyword, const Object &value) : keyword_(keyword), value_(value) {}

    const Object &value() const
    {
        return value_;
    }

    const Token &keyword() const
    {
        return keyword_;
    }

  private:
    Token keyword_;
    Object value_;
};

class Continue : public std::exception
{
  public:
    Continue(const Token &keyword) : keyword_(keyword) {}

    const Token &keyword() const
    {
        return keyword_;
    }

  private:
    Token keyword_;
};

class Break : public std::exception
{
  public:
    Break(const Token &keyword) : keyword_(keyword) {}

    const Token &keyword() const
    {
        return keyword_;
    }

  private:
    Token keyword_;
};
} // namespace control
} // namespace lox