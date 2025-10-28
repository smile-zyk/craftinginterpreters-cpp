#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "object.h"
#include "token.h"

namespace lox
{
class Environment
{
  public:
    Environment() : enclosing_(nullptr) {}
    Environment(Environment* enclosing) : enclosing_(enclosing) {}
    void Define(const std::string &name, const Object &value);
    const Object &Get(const Token &name);
    void Assign(const Token& name, const Object& value);

  private:
    std::unordered_map<std::string, Object> values_;
    Environment* enclosing_;
};

using EnvironmentUniquePtr = std::unique_ptr<Environment>;
} // namespace lox