#pragma once

#include <memory>
#include <vector>

#include "token.h"
#include "ast.h"


namespace lox
{
class Parser
{
public:
    Parser(const std::vector<Token>& tokens): tokens_(tokens){}

private:
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

private:
    // helper func
    bool Match(std::initializer_list<Token::Type> types);
    bool Match(Token::Type type);
    bool Check(Token::Type type);
    Token Advance();
    bool IsAtEnd();
    Token Peek();
    Token Previous();
    
private:
    size_t current_ = 0;
    const std::vector<Token>& tokens_;
};
}

