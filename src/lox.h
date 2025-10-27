#pragma once
#include <string>

#include "interpreter.h"

namespace lox
{
class Lox
{
  public:
    static void RunFile(const std::string &path);
    static void RunPrompt();
    static void Run(const std::string &source);
  public:
    static Interpreter interpreter;
};
} // namespace lox