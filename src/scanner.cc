
#include "scanner.h"
#include "lox.h"
#include "token.h"
#include <string>


using namespace lox;

Scanner::Scanner(const std::string &source) : source_(source) {}

const std::vector<Token> &Scanner::ScanTokens()
{
    while (!IsAtEnd())
    {
        start_ = current_;
        ScanToken();
    }

    tokens_.push_back(Token(Token::Type::kEOF, "", Token::LiteralNull(), line_));

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
        if(Match('/'))
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
        String();
        break;
    default:
        Lox::Error(line_, std::string("Unexpected character: ") + c + " .");
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
    if(IsAtEnd())
    {
        return '\0';
    }
    return source_.at(current_);
}

char Scanner::PeekNext()
{
    if(current_ + 1 >= source_.length())
    {
        return '\0';
    }
    return source_.at(current_ + 1);
}

bool Scanner::Match(char expected)
{
    if(Peek() != expected)
    {
        return false;
    }

    current_++;
    return true;
}

void Scanner::String()
{
    while(Peek() != '"' && !IsAtEnd())
    {
        if(Peek() == '\n')
        {
            line_++;
        }
        Advance();
    }

    std::string value = source_.substr(start_ + 1, current_ - (start_ + 1));
    
    if (IsAtEnd()) 
    {
      Lox::Error(line_, "Unterminated string: " + value + " .");
      return;
    }

    Advance();

    AddToken(Token::Type::kString, value);
}

void Scanner::Number()
{
    
}

void Scanner::AddToken(Token::Type type)
{
    AddToken(type, Token::LiteralNull());
}

void Scanner::AddToken(Token::Type type, Token::Literal literal)
{
    std::string text = source_.substr(start_, current_ - start_);
    tokens_.push_back(Token(type, text, literal, line_));
}