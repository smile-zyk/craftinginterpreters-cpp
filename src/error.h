#pragma once

#include <stdexcept>

#include "token.h"

namespace lox
{

class RuntimeError : public std::runtime_error
{
  public:
    explicit RuntimeError(const Token &token, const std::string &message) : std::runtime_error(message), token_(token)
    {
    }
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
    explicit ParseError(const Token &token, const std::string &message) : std::runtime_error(message), token_(token) {}
    const Token &token() const
    {
        return token_;
    }

  private:
    Token token_;
};

void Error(Token token, const std::string &message);
void Error(size_t line, const std::string &message);
void Error(const ParseError &e);
void Error(const RuntimeError &e);
void Report(size_t line, const std::string &where, const std::string &message);

extern bool had_error;
extern bool had_runtime_error;

} // namespace lox