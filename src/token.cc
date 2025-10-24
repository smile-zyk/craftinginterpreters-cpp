#include "token.h"
#include "object.h"

#include <magic_enum/magic_enum.hpp>

using namespace lox;

std::string Token::TypeToString(Type type)
{
    std::string type_name = std::string(magic_enum::enum_name(type));
    type_name.erase(0, 1);
    return type_name;
}

std::string Token::ToString() const
{
    return TypeToString(type_) + " " + lexeme_ + " " + lox::ObjectToString(literal_);
}