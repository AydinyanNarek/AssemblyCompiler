#include <iostream>
#include "../../include/interpretator/Interpretator.hpp"
#include "../../Utilities/include/ErrorManager/ErrorRegister.h"


int main(int args, char** argv) try {

    Interpreter::registerErrors();
    if (args != 2) {
        Errors::ErrorRegister::Throw("FileAttachError");
    }
    std::string file = argv[1];    
    Interpreter input(file);
    std::cout << "End..." << std::endl;
    return 0; 
}
catch (const char* e) {
    std::cout << e;
    return -1;
}
catch (const std::string& e) {
    std::cout << e;
    return -1;
}
catch (...) {
    std::cout << "Unoccured exeption was thrown!";
    return -1;
}