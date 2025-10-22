#pragma once
#include <string>

namespace lox
{

class Lox
{
public:
    static void RunFile(const std::string& path);
    static void RunPrompt();
    static void Run(const std::string& source);
    static void Error(size_t line, const std::string& message);
    static void Report(size_t line, const std::string& where, const std::string& message);
    
public:
    static bool had_error;
};
}