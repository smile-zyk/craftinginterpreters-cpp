#include "token.h"

#include <sstream>
#include <magic_enum/magic_enum.hpp>

using namespace lox;

std::string Token::LiteralToString(const Literal &literal)
{
    return std::visit(
        [](auto &&arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>)
            {
                return "null";
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                std::ostringstream oss;
                oss << arg;
                return oss.str();            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return arg;
            }
        },
        literal
    );
}

Token::Literal Token::LiteralNull()
{
    static Literal null;
    return null;
}

std::string Token::TypeToString(Type type)
{
    std::string type_name = std::string(magic_enum::enum_name(type));
    type_name.erase(0, 1);
    return type_name;
}

std::string Token::ToString() const
{
    return TypeToString(type_) + " " + lexeme_ + " " + LiteralToString(literal_);
}