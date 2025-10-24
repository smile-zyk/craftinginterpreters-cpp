#include "object.h"

#include <sstream>

namespace lox
{
std::string ObjectToString(const Object &obj)
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