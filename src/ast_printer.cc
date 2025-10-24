#include "ast_printer.h"

using namespace lox;

std::string AstPrinter::Print(Expr *expr) 
{
    Object res = expr->Accept(this);
    if(std::holds_alternative<std::string>(res))
    {
        return std::get<std::string>(res);
    }
    else {
        return "";
    }
}

Object AstPrinter::Visit(Binary *binary) 
{
    
}

Object AstPrinter::Visit(Grouping *binary) {}

Object AstPrinter::Visit(Literal *binary) {}

Object AstPrinter::Visit(Unary *binary) {}