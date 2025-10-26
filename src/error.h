#pragma once

#include <stdexcept>

#include "token.h"

namespace lox
{

class RuntimeError : public std::runtime_error
{
  public:
    RuntimeError(const Token &token, const std::string &message) : std::runtime_error(message), token_(token) {}
    const Token &token() const
    {
        return token_;
    }

  private:
    Token token_;
};

class ParseError : public std::runtime_error
{
  public:
    explicit ParseError(const std::string &message) : std::runtime_error(message) {}
};
} // namespace lox