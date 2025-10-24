#pragma once

#include "ast.h"

namespace lox {
class AstPrinter : public ExprVisitor
{
 public:
    std::string Print(Expr* expr);
    Object Visit(Binary* binary) override;
    Object Visit(Grouping* binary) override;
    Object Visit(Literal* binary) override;
    Object Visit(Unary* binary) override;
};
}