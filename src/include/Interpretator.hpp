//
//  Interpreter.hpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#pragma once
#include <vector>
#include "BaseCommands.hpp"
#include "Input.hpp"
#include "VirtualCpu.hpp"

/**
*   @brief Class Interpreter with all functionality 
**/
class Interpreter {

public:
    /**
     *   @brief Parametrized constructor of Interpreter class
     *   @param[in] file_name - input file
    **/
    explicit Interpreter(const std::string& file_name);

    /**
     *   @brief Destructor of Interpreter class
     *   @param[in] file_name - input file
    **/
    ~Interpreter() = default;

private:
    /**
     *   @brief Initiliaze class Input and class Compiler and take an output binary code 
     *   @param[in] file_name - input file
    **/
    void init(const std::string& file_name);

    /**
     *   @brief Interpret already got binary code  
    **/
    void interpreter();

private:
    VitualCPU m_cpu;
    std::vector<std::unique_ptr<BaseCommand>> m_instruction_table;
};
