#include <iostream>

#include "lox.h"

using namespace lox;

int main(int argc, char** argv)
{
    if(argc > 2)
    {
        std::cout << "Usage: lox-cpp [script]" << std::endl;
    }
    else if(argc == 2)
    {
        Lox::RunFile(argv[1]);
    }
    else 
    {
        Lox::RunPrompt();
    }
}