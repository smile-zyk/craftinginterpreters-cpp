#include "lox.h"

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "scanner.h"
#include "parser.h"
#include "token.h"
#include "ast_printer.h"

using namespace lox;

bool Lox::had_error = false;
bool Lox::had_runtime_error = false;
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
    std::unique_ptr<Expr> expr = parser.Parse();

    if(had_error) return;
    
    // AstPrinter printer;

    // std::cout << printer.Print(expr.get()) << std::endl;

    interpreter.Interpret(expr.get());
}

void Lox::Error(Token token, const std::string& message)
{
    if(token.type() == Token::Type::kEOF)
    {
        Report(token.line(), " at end", message);
    }
    else 
    {
        Report(token.line(), " at '" + token.lexeme() + "'", message);
    }
}

void Lox::Error(size_t line, const std::string& message)
{
    Report(line, "", message);
}

void Lox::Error(const RuntimeError& e)
{
    std::cerr << std::string(e.what()) + "\n[line " << e.token().line() << "]" << std::endl;
    had_runtime_error = true;
}

void Lox::Report(size_t line, const std::string& where, const std::string& message)
{
    std::cerr << "[line " << line << "] Error" + where + ": " + message << std::endl;
    had_error = true;
}
