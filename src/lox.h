#pragma once
#include <cstddef>
#include <string>

#include "token.h"
#include "error.h"
#include "interpreter.h"

namespace lox
{
class Lox
{
  public:
    static void RunFile(const std::string &path);
    static void RunPrompt();
    static void Run(const std::string &source);
    static void Error(Token token, const std::string &message);
    static void Error(size_t line, const std::string &message);
    static void Error(const RuntimeError& e);
    static void Report(size_t line, const std::string &where, const std::string &message);
  public:
    static bool had_error;
    static bool had_runtime_error;
    static Interpreter interpreter;
};
} // namespace lox