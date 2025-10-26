#include "ast_printer.h"

using namespace lox;

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

std::string AstPrinter::Parenthesize(const std::string& name, std::initializer_list<Expr*> exprs)
{
    std::string res;
    res = "(" + name;
    for(Expr* expr : exprs)
    {
        res += " ";
        res += ObjectToString(expr->Accept(this));
    }
    res += ")";
    return res;
}