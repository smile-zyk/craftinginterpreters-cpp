#pragma once

#include <variant>
#include <string>
#include <memory>

namespace lox
{
class Callable;
using CallablePtr = std::shared_ptr<Callable>;
using Object = std::variant<std::nullptr_t, double, bool, std::string, CallablePtr>;

std::string ObjectToString(const Object& obj);

template <typename T>
bool IsObjectInstance(const Object& obj)
{
    return std::holds_alternative<T>(obj);
}
}