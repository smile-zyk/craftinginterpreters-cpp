
#include "scanner.h"
#include "object.h"
#include "token.h"
#include "error.h"
#include <string>

using namespace lox;

std::unordered_map<std::string, Token::Type> Scanner::keywords_ = {
    {"and", Token::Type::kAnd},     {"class", Token::Type::kClass},   {"else", Token::Type::kElse},
    {"false", Token::Type::kFalse}, {"for", Token::Type::kFor},       {"fun", Token::Type::kFun},
    {"if", Token::Type::kIf},       {"nil", Token::Type::kNil},       {"or", Token::Type::kOr},
    {"print", Token::Type::kPrint}, {"return", Token::Type::kReturn}, {"super", Token::Type::kSuper},
    {"this", Token::Type::kThis},   {"true", Token::Type::kTrue},     {"var", Token::Type::kVar},
    {"while", Token::Type::kWhile}};

Scanner::Scanner(const std::string &source) : source_(source) {}

const std::vector<Token> &Scanner::ScanTokens()
{
    while (!IsAtEnd())
    {
        start_ = current_;
        ScanToken();
    }

    tokens_.push_back(Token(Token::Type::kEOF, "", nullptr, line_));

    return tokens_;
}

void Scanner::ScanToken()
{
    const char c = Advance();
    switch (c)
    {
    case '(':
        AddToken(Token::Type::kLeftParen);
        break;
    case ')':
        AddToken(Token::Type::kRightParen);
        break;
    case '{':
        AddToken(Token::Type::kLeftBrace);
        break;
    case '}':
        AddToken(Token::Type::kRightBrace);
        break;
    case ',':
        AddToken(Token::Type::kComma);
        break;
    case '.':
        AddToken(Token::Type::kDot);
        break;
    case '-':
        AddToken(Token::Type::kMinus);
        break;
    case '+':
        AddToken(Token::Type::kPlus);
        break;
    case ';':
        AddToken(Token::Type::kSemicolon);
        break;
    case '*':
        AddToken(Token::Type::kStar);
        break;
    case '!':
        AddToken(Match('=') ? Token::Type::kBangEqual : Token::Type::kBang);
        break;
    case '=':
        AddToken(Match('=') ? Token::Type::kEqualEqual : Token::Type::kEqual);
        break;
    case '<':
        AddToken(Match('=') ? Token::Type::kLessEqual : Token::Type::kLess);
        break;
    case '>':
        AddToken(Match('=') ? Token::Type::kGreaterEqual : Token::Type::kGreater);
        break;
    case '/':
        if (Match('/'))
        {
            while (Peek() != '\n' && !IsAtEnd())
            {
                Advance();
            }
        }
        else
        {
            AddToken(Token::Type::kSlash);
        }
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line_++;
        break;
    case '"':
        string();
        break;
    default:
        if (IsDigit(c))
        {
            number();
        }
        else if(IsAlpha(c))
        {
            identifier();
        }
        else
        {
            lox::Error(line_, std::string("Unexpected character: ") + c + " .");
        }
        break;
    }
}

bool Scanner::IsAtEnd()
{
    return current_ >= source_.size();
}

char Scanner::Advance()
{
    current_++;
    return source_.at(current_ - 1);
}

char Scanner::Peek()
{
    if (IsAtEnd())
    {
        return '\0';
    }
    return source_.at(current_);
}

char Scanner::PeekNext()
{
    if (current_ + 1 >= source_.length())
    {
        return '\0';
    }
    return source_.at(current_ + 1);
}

bool Scanner::Match(char expected)
{
    if (Peek() != expected)
    {
        return false;
    }

    current_++;
    return true;
}

bool Scanner::IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Scanner::IsAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

void Scanner::string()
{
    while (Peek() != '"' && !IsAtEnd())
    {
        if (Peek() == '\n')
        {
            line_++;
        }
        Advance();
    }

    std::string value = source_.substr(start_ + 1, current_ - (start_ + 1));

    if (IsAtEnd())
    {
        lox::Error(line_, "Unterminated string: " + value + " .");
        return;
    }

    Advance();

    AddToken(Token::Type::kString, value);
}

void Scanner::number()
{
    while (IsDigit(Peek()))
    {
        Advance();
    }

    if (Peek() == '.' && IsDigit(PeekNext()))
    {
        Advance();

        while (IsDigit(Peek()))
        {
            Advance();
        }
    }

    std::string num_str = source_.substr(start_, current_ - start_);
    AddToken(Token::Type::kNumber, std::stod(num_str));
}

void Scanner::identifier()
{
    while (IsAlpha(Peek()) || IsDigit(Peek()))
    {
        Advance();
    }

    std::string text = source_.substr(start_, current_ - start_);
    Token::Type type = Token::Type::kIdentifier;
    auto type_it = keywords_.find(text);
    if(type_it != keywords_.end())
    {
        type = type_it->second;
    }

    AddToken(type);
}

void Scanner::AddToken(Token::Type type)
{
    AddToken(type, nullptr);
}

void Scanner::AddToken(Token::Type type, Object literal)
{
    std::string text = source_.substr(start_, current_ - start_);
    tokens_.push_back(Token(type, text, literal, line_));
}