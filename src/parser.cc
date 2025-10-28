#include "parser.h"
#include "ast.h"
#include "error.h"
#include "token.h"
#include <memory>

using namespace lox;
using namespace lox::expr;
using namespace lox::stmt;

Program Parser::Parse()
{
    try
    {
        return program();
    }
    catch (const ParseError &e)
    {
        lox::Error(e);
        return {};
    }
}

/*
program        → declaration* EOF ;

declaration    → varDecl
               | statement ;

statement      → exprStmt
               | printStmt 
               | block ;

block          → "{" declaration* "}" ;

exprStmt       → expression ";" ;
printStmt      → "print" expression ";" ;
*/

// program        → statement* EOF ;
Program Parser::program()
{
    std::vector<StmtUniquePtr> statements;

    while (!IsAtEnd())
    {
        statements.push_back(std::move(declaration()));
    }

    return statements;
}

// declaration    → varDecl
//                | statement ;
StmtUniquePtr Parser::declaration()
{
    try
    {
        if (Match(Token::Type::kVar))
        {
            return var_declaration();
        }
        return statement();
    }
    catch (const ParseError &e)
    {
        lox::Error(e);
        Synchronize();
        return nullptr;
    }
}

// varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
StmtUniquePtr Parser::var_declaration() 
{
    Token name = Consume(Token::Type::kIdentifier, "Expect variable name.");

    ExprUniquePtr initializer = nullptr;
    if(Match(Token::Type::kEqual))
    {
        initializer = expression();
    }

    Consume(Token::Type::kSemicolon, "Expect ';' after variable declaration.");
    return std::make_unique<Var>(name, std::move(initializer));
}

// statement      → exprStmt
//                | printStmt ;
//                | block ;
StmtUniquePtr Parser::statement()
{
    if (Match(Token::Type::kPrint))
    {
        return print_statment();
    }
    if (Match(Token::Type::kLeftBrace))
    {
        return block();
    }
    return expression_statment();
}

// block          → "{" declaration* "}" ;
StmtUniquePtr Parser::block()
{
    StmtList statements;
    
    while(!Check(Token::Type::kRightBrace) && !IsAtEnd())
    {
        statements.push_back(declaration());
    }
    
    Consume(Token::Type::kRightBrace, "Expect '}' after block.");

    return std::make_unique<Block>(std::move(statements));
}

// exprStmt       → expression ";" ;
StmtUniquePtr Parser::print_statment()
{
    ExprUniquePtr value = expression();
    Consume(Token::Type::kSemicolon, "Expect ';' after value.");
    return std::make_unique<Print>(std::move(value));
}

// printStmt      → "print" expression ";" ;
StmtUniquePtr Parser::expression_statment()
{
    ExprUniquePtr expr = expression();
    Consume(Token::Type::kSemicolon, "Expect ';' after expression.");
    return std::make_unique<Expression>(std::move(expr));
}

/*
expression     → assignment ;
assignment     → IDENTIFIER "=" assignment
               | equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")"
               | IDENTIFIER ;
*/

// expression     → assignment ;
ExprUniquePtr Parser::expression()
{
    return assignment();
}

// assignment     → IDENTIFIER "=" assignment
//                | equality ;
ExprUniquePtr Parser::assignment()
{
    auto expr = equality();
    
    if(Match(Token::Type::kEqual))
    {
        Token equals = Previous();
        auto value = assignment();

        Variable* var = dynamic_cast<Variable*>(expr.get());
        if(var != nullptr)
        {
            Token name = var->name();
            return std::make_unique<Assign>(name, std::move(value));
        }

        throw ParseError(equals, "Invalid assignment target.");
    }

    return expr;
}

// equality       → comparison ( ( "!=" | "==" ) comparison )* ;
ExprUniquePtr Parser::equality()
{
    auto expr = comparison();

    static const auto kEqualityOps = {Token::Type::kBangEqual, Token::Type::kEqualEqual};

    while (Match(kEqualityOps))
    {
        Token oper = Previous();
        ExprUniquePtr right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

// comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
ExprUniquePtr Parser::comparison()
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
ExprUniquePtr Parser::term()
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
ExprUniquePtr Parser::factor()
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
ExprUniquePtr Parser::unary()
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
ExprUniquePtr Parser::primary()
{
    if (Match(Token::Type::kTrue))
    {
        return std::make_unique<Literal>(true);
    }

    if (Match(Token::Type::kFalse))
    {
        return std::make_unique<Literal>(false);
    }

    if (Match(Token::Type::kNil))
    {
        return std::make_unique<Literal>(nullptr);
    }

    static const auto kPrimaryOps = {Token::Type::kNumber, Token::Type::kString};

    if (Match(kPrimaryOps))
    {
        return std::make_unique<Literal>(Previous().literal());
    }

    if (Match(Token::Type::kIdentifier))
    {
        return std::make_unique<Variable>(Previous());
    }

    if (Match(Token::Type::kLeftParen))
    {
        auto expr = expression();
        Consume(Token::Type::kRightParen, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw ParseError(Peek(), "Expect expression.");
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

Token Parser::Consume(Token::Type type, const std::string &message)
{
    if (Check(type))
    {
        return Advance();
    }

    throw ParseError(Peek(), message);
}

void Parser::Synchronize()
{
    Advance();

    while (!IsAtEnd())
    {
        if (Previous().type() == Token::Type::kSemicolon)
        {
            return;
        }

        switch (Peek().type())
        {
        case Token::Type::kClass:
        case Token::Type::kFun:
        case Token::Type::kVar:
        case Token::Type::kFor:
        case Token::Type::kIf:
        case Token::Type::kWhile:
        case Token::Type::kPrint:
        case Token::Type::kReturn:
            return;
        default:
            break;
        }

        Advance();
    }
}