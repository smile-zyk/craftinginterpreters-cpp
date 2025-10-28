#include "object.h"
#include "callable.h"

#include <cstddef>
#include <sstream>

namespace lox
{
std::string ObjectToString(const Object &obj)
{
    return std::visit(
        [](auto &&arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::nullptr_t>)
            {
                return "nil";
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                std::ostringstream oss;
                oss << std::fixed << arg;
                return oss.str();
            }
            else if constexpr (std::is_same_v<T, bool>) 
            {
                return arg ? "true" : "false";   
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return arg;
            }
            else if constexpr (std::is_same_v<T, CallablePtr>)
            {
                return arg->ToString();
            }
            return "";
        },
        obj
    );
}

Object ObjectNull()
{
    static Object obj;
    return obj;
}

} // namespace lox