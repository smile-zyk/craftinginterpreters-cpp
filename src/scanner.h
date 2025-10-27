#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "token.h"
#include "object.h"

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

    bool IsDigit(char c);
    bool IsAlpha(char c);

    void string();
    void number();
    void identifier();

    void AddToken(Token::Type type);
    void AddToken(Token::Type type, Object literal);
  private:
    static std::unordered_map<std::string, Token::Type> keywords_;
    std::string source_;
    std::vector<Token> tokens_;
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
};

} // namespace lox