#include "error.h"

#include <iostream>

namespace lox
{
bool had_error = false;
bool had_runtime_error = false;

void Error(Token token, const std::string &message)
{
    if (token.type() == Token::Type::kEOF)
    {
        Report(token.line(), "at end", message);
    }
    else
    {
        Report(token.line(), "at '" + token.lexeme() + "'", message);
    }
}

void Error(size_t line, const std::string &message)
{
    Report(line, "", message);
}

void Error(const ParseError &e)
{
    Error(e.token(), e.what());
    had_runtime_error = true;
}

void Error(const RuntimeError &e)
{
    Error(e.token(), e.what());
    had_runtime_error = true;
}

void Report(size_t line, const std::string &where, const std::string &message)
{
    std::cerr << "[line " << line << "] Error " + where + ": " + message << std::endl;
    had_error = true;
}

} // namespace lox