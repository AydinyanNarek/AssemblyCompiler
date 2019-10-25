//
//  Interpreter.cpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#include "../include/Interpretator.hpp"
#include "../include/Input.hpp"
#include "../include/Compiler.hpp"
#include <memory>

Interpreter::Interpreter(const std::string& file_name) {
    init(file_name);
    interpreter();
    Compiler::registerErrors();
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
