#include "ast_printer.h"
#include "ast.h"
#include "object.h"

using namespace lox;
using namespace lox::expr;

std::string AstPrinter::Print(Expr *expr)
{
    return ObjectToString(expr->Accept(this));
}

Object AstPrinter::Visit(Binary *expr)
{
    return Parenthesize(expr->oper().lexeme(), {expr->left(), expr->right()});
}

Object AstPrinter::Visit(Grouping *expr)
{
    return Parenthesize("group", {expr->expression()});
}

Object AstPrinter::Visit(Literal *expr)
{
    return ObjectToString(expr->value());
}

Object AstPrinter::Visit(Unary *expr)
{
    return Parenthesize(expr->oper().lexeme(), {expr->right()});
}

Object AstPrinter::Visit(Variable *expr)
{
    return Parenthesize(expr->name().lexeme(), {});
}

Object AstPrinter::Visit(Assign *expr)
{
    return Parenthesize(expr->name().lexeme(), {expr->value()});
}

std::string AstPrinter::Parenthesize(const std::string &name, std::initializer_list<Expr *> exprs)
{
    std::string res;
    res = "(" + name;
    for (Expr *expr : exprs)
    {
        res += " ";
        res += ObjectToString(expr->Accept(this));
    }
    res += ")";
    return res;
}