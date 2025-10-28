#pragma once

#include <vector>

#include "token.h"
#include "ast.h"

namespace lox
{
class Parser
{
public:
    Parser(const std::vector<Token>& tokens): tokens_(tokens){}

    Program Parse();
    
private:
    // parse stmt
    Program program();
    StmtUniquePtr declaration();
    StmtUniquePtr var_declaration();
    StmtUniquePtr statement();
    StmtUniquePtr print_statment();
    StmtUniquePtr expression_statment();
    StmtUniquePtr block();

    // parse expr
    ExprUniquePtr expression();
    ExprUniquePtr assignment();
    ExprUniquePtr equality();
    ExprUniquePtr comparison();
    ExprUniquePtr term();
    ExprUniquePtr factor();
    ExprUniquePtr unary();
    ExprUniquePtr primary();

private:
    // helper func
    bool Match(std::initializer_list<Token::Type> types);
    bool Match(Token::Type type);
    bool Check(Token::Type type);
    Token Advance();
    bool IsAtEnd();
    Token Peek();
    Token Previous();
    Token Consume(Token::Type type, const std::string& message);
    void Synchronize();

private:
    size_t current_ = 0;
    const std::vector<Token>& tokens_;
};
}

