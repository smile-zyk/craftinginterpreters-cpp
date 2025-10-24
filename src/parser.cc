#include "parser.h"
#include "ast.h"
#include "token.h"
#include <memory>
#include <utility>

using namespace lox;

/*
expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
*/

// expression     → equality ;
std::unique_ptr<Expr> Parser::expression()
{
    return equality();
}

// equality       → comparison ( ( "!=" | "==" ) comparison )* ;
std::unique_ptr<Expr> Parser::equality()
{
    auto expr = comparison();

    static const auto kEqualityOps = {Token::Type::kBangEqual, Token::Type::kEqualEqual};

    while (Match(kEqualityOps))
    {
        Token oper = Previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

// comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
std::unique_ptr<Expr> Parser::comparison()
{
    auto expr = term();

    static const auto kComparisonOps = {
        Token::Type::kGreater, Token::Type::kGreaterEqual, Token::Type::kLess, Token::Type::kLessEqual
    };

    while (Match(kComparisonOps))
    {
        Token oper = Previous();
        auto right = term();
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

// term           → factor ( ( "-" | "+" ) factor )* ;
std::unique_ptr<Expr> Parser::term()
{
    auto expr = factor();

    static const auto kTermOps = {Token::Type::kPlus, Token::Type::kMinus};

    while (Match(kTermOps))
    {
        Token oper = Previous();
        auto right = factor();
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

// factor         → unary ( ( "/" | "*" ) unary )* ;
std::unique_ptr<Expr> Parser::factor()
{
    auto expr = unary();

    static const auto kFactorOps = {Token::Type::kSlash, Token::Type::kStar};

    while (Match(kFactorOps))
    {
        Token oper = Previous();
        auto right = unary();
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

// unary          → ( "!" | "-" ) unary
//                | primary ;
std::unique_ptr<Expr> Parser::unary()
{
    static const auto kUnaryOps = {Token::Type::kBang, Token::Type::kMinus};
    if (Match(kUnaryOps))
    {
        Token oper = Previous();
        auto right = unary();
        return std::make_unique<Unary>(oper, std::move(right));
    }

    return primary();
}

// primary        → NUMBER | STRING | "true" | "false" | "nil"
//                | "(" expression ")" ;
std::unique_ptr<Expr> Parser::primary()
{
    if (Match(Token::Type::kTrue))
    {
        return std::make_unique<Literal>(true);
    }

    if (Match(Token::Type::kFalse))
    {
        return std::make_unique<Literal>(false);
    }
    
    if(Match(Token::Type::kNil))
    {
        return std::make_unique<Literal>(ObjectNull());
    }

    static const auto kPrimaryOps = {Token::Type::kNumber, Token::Type::kString};

    if(Match(kPrimaryOps))
    {
        return std::make_unique<Literal>(Previous().literal());
    }

    if(Match(Token::Type::kLeftParen))
    {
        auto expr = expression();
        return std::make_unique<Grouping>(std::move(expr));
    }
}

bool Parser::Match(std::initializer_list<Token::Type> types)
{
    for (auto type : types)
    {
        if (Check(type))
        {
            Advance();
            return true;
        }
    }

    return false;
}

bool Parser::Match(Token::Type type)
{
    if (Check(type))
    {
        Advance();
        return true;
    }

    return false;
}

bool Parser::Check(Token::Type type)
{
    if (IsAtEnd())
    {
        return false;
    }
    return Peek().type() == type;
}

Token Parser::Advance()
{
    if (!IsAtEnd())
        current_++;
    return Previous();
}

bool Parser::IsAtEnd()
{
    return Peek().type() == Token::Type::kEOF;
}

Token Parser::Peek()
{
    return tokens_.at(current_);
}

Token Parser::Previous()
{
    return tokens_.at(current_ - 1);
}