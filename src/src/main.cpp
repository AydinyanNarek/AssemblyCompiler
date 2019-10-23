#include <iostream>
#include "../include/Interpretator.hpp"
#include "../include/Compiler.hpp"

int main() try
{
    Compiler::registerErrors();
    std::string file = "/home/nareka/Desktop/GITHUBProjects/AssemblyCompiler/tests/test.txt";
    Interpreter input(file);
    std::cout << "End..." << std::endl;
    return 0; 
}
catch (const char* e)
{
    std::cout << e;
    return -1;
}
catch (const std::string& e)
{
    std::cout << e;
    return -1;
}
catch (...)
{
    std::cout << "Unoccured exeption was thrown!";
    return -1;
}