#include "lox.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "ast.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"
//#include "ast_printer.h"

using namespace lox;
using namespace lox::expr;

Interpreter Lox::interpreter;

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
    if(had_runtime_error)
    {
        exit(-1);
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
    Parser parser(tokens);
    Program program = parser.Parse();

    if(had_error) return;

    interpreter.Interpret(program);
}