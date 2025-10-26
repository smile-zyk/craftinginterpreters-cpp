#pragma once

#include <variant>
#include <string>

namespace lox
{
using Object = std::variant<std::nullptr_t, double, bool, std::string>;

std::string ObjectToString(const Object& obj);

template <typename T>
bool IsObjectInstance(const Object& obj)
{
    return std::holds_alternative<T>(obj);
}
}