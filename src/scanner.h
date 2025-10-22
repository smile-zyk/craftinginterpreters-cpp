#pragma once

#include <cstddef>
#include <vector>

#include "token.h"

namespace lox
{

class Scanner
{
  public:
    Scanner(const std::string &source);
    const std::vector<Token>& ScanTokens();
    const std::vector<Token>& tokens() { return tokens_; }

  private:
    void ScanToken();
    bool IsAtEnd();
    char Advance();
    char Peek();
    char PeekNext();
    bool Match(char expected);
    void String();
    void Number();
    void AddToken(Token::Type type);
    void AddToken(Token::Type type, Token::Literal literal);
  private:
    std::string source_;
    std::vector<Token> tokens_;
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
};

} // namespace lox