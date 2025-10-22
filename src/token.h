#pragma once
#include <string>
#include <variant>

namespace lox
{

class Token
{
  public:
    enum class Type
    {
        // Single-character tokens.
        kLeftParen,  // LEFT_PAREN
        kRightParen, // RIGHT_PAREN
        kLeftBrace,  // LEFT_BRACE
        kRightBrace, // RIGHT_BRACE
        kComma,      // COMMA
        kDot,        // DOT
        kMinus,      // MINUS
        kPlus,       // PLUS
        kSemicolon,  // SEMICOLON
        kSlash,      // SLASH
        kStar,       // STAR

        // One or two character tokens.
        kBang,         // BANG
        kBangEqual,    // BANG_EQUAL
        kEqual,        // EQUAL
        kEqualEqual,   // EQUAL_EQUAL
        kGreater,      // GREATER
        kGreaterEqual, // GREATER_EQUAL
        kLess,         // LESS
        kLessEqual,    // LESS_EQUAL

        // Literals.
        kIdentifier, // IDENTIFIER
        kString,     // STRING
        kNumber,     // NUMBER

        // Keywords.
        kAnd,    // AND
        kClass,  // CLASS
        kElse,   // ELSE
        kFalse,  // FALSE
        kFun,    // FUN
        kFor,    // FOR
        kIf,     // IF
        kNil,    // NIL
        kOr,     // OR
        kPrint,  // PRINT
        kReturn, // RETURN
        kSuper,  // SUPER
        kThis,   // THIS
        kTrue,   // TRUE
        kVar,    // VAR
        kWhile,  // WHILE

        kEOF // EOF
    };

    using Literal = std::variant<std::monostate, int, double, std::string>;
    static Literal LiteralNull();
    static std::string LiteralToString(const Literal &literal);
    static std::string TypeToString(Type type);

    Token(Type type, const std::string &lexeme, Literal literal, int line)
        : type_(type), lexeme_(lexeme), literal_(literal), line_(line)
    {
    }

    std::string ToString() const;

    Type type() const
    {
        return type_;
    }

    const std::string &lexeme() const
    {
        return lexeme_;
    }

    Literal literal() const
    {
        return literal_;
    }
    
    int line() const
    {
        return line_;
    }

  private:
    Type type_;
    std::string lexeme_;
    Literal literal_;
    int line_;
};

} // namespace lox