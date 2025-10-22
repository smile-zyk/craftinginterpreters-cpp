#include "lox.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "scanner.h"
#include "token.h"

using namespace lox;

bool Lox::had_error = false;

void Lox::RunFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("can not open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    Run(code);
    if (had_error)
    {
        exit(1);
    }
}

void Lox::RunPrompt()
{
    std::string line;
    while (true)
    {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
        {
            break;
        }
        Run(line);
        had_error = false;
    }
}

void Lox::Run(const std::string &source)
{
    Scanner scanner(source);
    auto tokens = scanner.ScanTokens();

    for (const Token &token : tokens)
    {
        std::cout << token.ToString() << std::endl;
    }
}

void Lox::Error(int line, const std::string& message)
{
    Report(line, "", message);
}

void Lox::Report(int line, const std::string& where, const std::string& message)
{
    std::cerr << "[line " << line << "] Error" + where + ": " + message << std::endl;
    had_error = true;
}
