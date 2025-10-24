#pragma once

#include <variant>
#include <string>

namespace lox
{
using Object = std::variant<std::monostate, double, bool, std::string>;

std::string ObjectToString(const Object& obj);

Object ObjectNull();
}