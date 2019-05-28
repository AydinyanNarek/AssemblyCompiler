#include <iostream>
#include "Interpretator.hpp"
#include "Compiler.hpp"

int main () try
{
    std::string file = "/home/narek/Desktop/Asembler/AssemblyCompiler/tests/test.txt";
    Interpreter input(file);
    std::cout << "End..." << std::endl;
    return 0;
}
catch (const char* e)
{
    std::cout << e;
    return -1;
}
catch (...)
{
    std::cout << "Unoccured exeption was thrown";
    return -1;
}