#include "parser.h"
#include "ast.h"
#include "error.h"
#include "token.h"
#include <memory>
#include <utility>

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

declaration    → funDecl
               | varDecl
               | statement ;

funDecl        → "fun" function ;
function       → IDENTIFIER "(" parameters? ")" block ;

parameters     → IDENTIFIER ( "," IDENTIFIER )* ;

statement      → exprStmt
               | forStmt
               | ifStmt
               | printStmt
               | returnStmt
               | whileStmt
               | block ;

returnStmt     → "return" expression? ";" ;

forStmt        → "for" "(" ( varDecl | exprStmt | ";" )
                 expression? ";"
                 expression? ")" statement ;

whileStmt      → "while" "(" expression ")" statement ;

ifStmt         → "if" "(" expression ")" statement
               ( "else" statement )? ;

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

// declaration    → funDecl
//                | varDecl
//                | statement ;
StmtUniquePtr Parser::declaration()
{
    try
    {
        if(Match(Token::Type::kFun))
        {
            return func_declaration("function");
        }
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
    if (Match(Token::Type::kEqual))
    {
        initializer = expression();
    }

    Consume(Token::Type::kSemicolon, "Expect ';' after variable declaration.");
    return std::make_unique<Var>(name, std::move(initializer));
}

//function       → IDENTIFIER "(" parameters? ")" block ;
StmtUniquePtr Parser::func_declaration(const std::string& kind)
{
    Token name = Consume(Token::Type::kIdentifier, "Expect " + kind + " name.");
    Consume(Token::Type::kLeftParen, "Expect '(' after " + kind + " name.");

    std::vector<Token> parameters ;
    if (!Check(Token::Type::kRightParen))
    {
        do
        {
            if (parameters .size() >= 255)
            {
                lox::Error(Peek(), "Can't have more than 255 arguments.");
            }
            parameters .push_back(Consume(Token::Type::kIdentifier, "Expect parameter name."));
        } while (Match(Token::Type::kComma));
    }

    Token paren = Consume(Token::Type::kRightParen, "Expect ')' after arguments.");

    Consume(Token::Type::kLeftBrace, "Expect '{' before " + kind + " body.");
    auto body = block();
    return std::make_unique<Function>(name, parameters, std::move(body));
}

// statement      → exprStmt
//                | forStmt
//                | ifStmt
//                | printStmt
//                | returnStmt
//                | whileStmt
//                | block ;
StmtUniquePtr Parser::statement()
{
    if (Match(Token::Type::kIf))
    {
        return if_statement();
    }
    if (Match(Token::Type::kPrint))
    {
        return print_statement();
    }
    if (Match(Token::Type::kReturn))
    {
        return return_statement();
    }
    if (Match(Token::Type::kWhile))
    {
        return while_statement();
    }
    if (Match(Token::Type::kFor))
    {
        return for_statement();
    }
    if (Match(Token::Type::kLeftBrace))
    {
        return block();
    }
    return expression_statment();
}

// returnStmt     → "return" expression? ";" ;
StmtUniquePtr Parser::return_statement()
{
    Token keyword = Previous();
    ExprUniquePtr value = nullptr;
    if(!Check(Token::Type::kSemicolon))
    {
        value = expression();
    }

    Consume(Token::Type::kSemicolon, "Expect ';' after return value.");
    return std::make_unique<Return>(keyword, std::move(value));
}

// exprStmt       → expression ";" ;
StmtUniquePtr Parser::expression_statment()
{
    ExprUniquePtr expr = expression();
    Consume(Token::Type::kSemicolon, "Expect ';' after expression.");
    return std::make_unique<Expression>(std::move(expr));
}

// ifStmt         → "if" "(" expression ")" statement
//                ( "else" statement )? ;
StmtUniquePtr Parser::if_statement()
{
    Consume(Token::Type::kLeftParen, "Expect '(' after 'if'.");
    ExprUniquePtr condition = expression();
    Consume(Token::Type::kRightParen, "Expect ')' after if condition.");

    StmtUniquePtr then_branch = statement();
    StmtUniquePtr else_branch = nullptr;
    if (Match(Token::Type::kElse))
    {
        else_branch = statement();
    }

    return std::make_unique<If>(std::move(condition), std::move(then_branch), std::move(else_branch));
}

// printStmt      → "print" expression ";" ;
StmtUniquePtr Parser::print_statement()
{
    ExprUniquePtr value = expression();
    Consume(Token::Type::kSemicolon, "Expect ';' after value.");
    return std::make_unique<Print>(std::move(value));
}

// whileStmt      → "while" "(" expression ")" statement ;
StmtUniquePtr Parser::while_statement()
{
    Consume(Token::Type::kLeftParen, "Expect '(' after 'while'.");
    ExprUniquePtr condition = expression();
    Consume(Token::Type::kRightParen, "Expect ')' after condition.");
    StmtUniquePtr body = statement();

    return std::make_unique<While>(std::move(condition), std::move(body));
}

// forStmt        → "for" "(" ( varDecl | exprStmt | ";" )
//                  expression? ";"
//                  expression? ")" statement ;
StmtUniquePtr Parser::for_statement()
{
    Consume(Token::Type::kLeftParen, "Expect '(' after 'for'.");

    StmtUniquePtr initializer;
    if (Match(Token::Type::kSemicolon))
    {
        initializer = nullptr;
    }
    else if (Match(Token::Type::kVar))
    {
        initializer = var_declaration();
    }
    else
    {
        initializer = expression_statment();
    }

    ExprUniquePtr condition = nullptr;
    if (!Check(Token::Type::kSemicolon))
    {
        condition = expression();
    }
    Consume(Token::Type::kSemicolon, "Expect ';' after loop condition.");

    ExprUniquePtr increment = nullptr;
    if (!Check(Token::Type::kRightParen))
    {
        increment = expression();
    }
    Consume(Token::Type::kRightParen, "Expect ')' after for clauses.");

    StmtUniquePtr body = statement();

    if (increment != nullptr)
    {
        StmtList statements;
        statements.push_back(std::move(body));
        statements.push_back(std::make_unique<Expression>(std::move(increment)));
        body = std::make_unique<Block>(std::move(statements));
    }

    if (condition == nullptr)
    {
        condition = std::make_unique<Literal>(true);
    }
    body = std::make_unique<While>(std::move(condition), std::move(body));

    if (initializer != nullptr)
    {
        StmtList statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<Block>(std::move(statements));
    }

    return body;
}

// block          → "{" declaration* "}" ;
StmtUniquePtr Parser::block()
{
    StmtList statements;

    while (!Check(Token::Type::kRightBrace) && !IsAtEnd())
    {
        statements.push_back(declaration());
    }

    Consume(Token::Type::kRightBrace, "Expect '}' after block.");

    return std::make_unique<Block>(std::move(statements));
}

/*
expression     → assignment ;
assignment     → IDENTIFIER "=" assignment
               | logic_or ;
logic_or       → logic_and ( "or" logic_and )* ;
logic_and      → equality ( "and" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary | call ;
call           → primary ( "(" arguments? ")" )* ;
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
//                | logic_or ;
ExprUniquePtr Parser::assignment()
{
    auto expr = logic_or();

    if (Match(Token::Type::kEqual))
    {
        Token equals = Previous();
        auto value = assignment();

        Variable *var = dynamic_cast<Variable *>(expr.get());
        if (var != nullptr)
        {
            Token name = var->name();
            return std::make_unique<Assign>(name, std::move(value));
        }

        throw ParseError(equals, "Invalid assignment target.");
    }

    return expr;
}

// logic_or       → logic_and ( "or" logic_and )* ;
ExprUniquePtr Parser::logic_or()
{
    auto expr = logic_and();

    while (Match(Token::Type::kOr))
    {
        Token oper = Previous();
        auto right = logic_and();
        expr = std::make_unique<Logical>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

// logic_and      → equality ( "and" equality )* ;
ExprUniquePtr Parser::logic_and()
{
    auto expr = equality();

    while (Match(Token::Type::kAnd))
    {
        Token oper = Previous();
        auto right = equality();
        expr = std::make_unique<Logical>(std::move(expr), oper, std::move(right));
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
        Token::Type::kGreater, Token::Type::kGreaterEqual, Token::Type::kLess, Token::Type::kLessEqual};

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

// unary          → ( "!" | "-" ) unary | call ;
ExprUniquePtr Parser::unary()
{
    static const auto kUnaryOps = {Token::Type::kBang, Token::Type::kMinus};
    if (Match(kUnaryOps))
    {
        Token oper = Previous();
        auto right = unary();
        return std::make_unique<Unary>(oper, std::move(right));
    }

    return call();
}

// call           → primary ( "(" arguments? ")" )* ;
ExprUniquePtr Parser::call()
{
    ExprUniquePtr expr = primary();

    while (true)
    {
        if (Match(Token::Type::kLeftParen))
        {
            expr = finish_call(std::move(expr));
        }
        else
        {
            break;
        }
    }

    return expr;
}

ExprUniquePtr Parser::finish_call(ExprUniquePtr callee)
{
    ExprList arguments;
    if (!Check(Token::Type::kRightParen))
    {
        do
        {
            if (arguments.size() >= 255)
            {
                lox::Error(Peek(), "Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (Match(Token::Type::kComma));
    }

    Token paren = Consume(Token::Type::kRightParen, "Expect ')' after arguments.");

    return std::make_unique<Call>(std::move(callee), paren, std::move(arguments));
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