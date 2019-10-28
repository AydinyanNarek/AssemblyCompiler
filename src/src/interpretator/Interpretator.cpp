//
//  Interpreter.cpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#include "../../include/interpretator/Interpretator.hpp"
#include "../../include/compiliator/Input.hpp"
#include "../../include/compiliator/Compiler.hpp"
#include <memory>

void Interpreter::registerErrors() {
    Errors::ErrorRegister::registerErrors({
        {"InvalidJumpError",            "Faild jump command."},
        {"UnregisteredCommandError",    "Faild to make command as it is not supported."},
        {"IncorrectFileError",          "Faild to open input file."},
        {"FileAttachError",             "Please specify input file."},
        {"UnregisteredTypeError",       "Faild to use the type as it is not supported."}
    });
}

Interpreter::Interpreter(const std::string& file_name) {
    init(file_name);
    interpreter();
}

void Interpreter::init(const std::string& file_name) {
    Input inp (file_name);
    inp.readDataFromFile();
    auto code = inp.getData();
    auto compile = std::make_unique<Compiler>(Compiler());
    m_instruction_table = compile->createCode(code);
}

void Interpreter::interpreter() {
    while (m_cpu.m_ip < m_instruction_table.size()) {
        const auto current_ip = m_cpu.m_ip;
        ++m_cpu.m_ip;
        m_instruction_table[current_ip]->execute(m_cpu);
    }
}
