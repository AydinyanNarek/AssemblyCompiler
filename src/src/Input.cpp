//
//  Input.cpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#include "../include/Input.hpp"
#include "../Utilities/include/ErrorManager/ErrorRegister.h"

Input::Input(std::string_view file_name)
    : m_filename(file_name), m_openQ(false) {}

Input::~Input() {
    if (m_openQ) {
        (m_buffer).close();
    }
}

void Input::readDataFromFile() {
    (m_buffer).seekg(0);
    std::string line;
    (m_buffer).open(m_filename, std::fstream::in);
    m_openQ = true;

    if(!(m_buffer).is_open()){
        Errors::ErrorRegister::Throw("IncorrectFileError");
    }
    while (getline((m_buffer), line))
    {
        if (line.empty()) {
           continue;
        }
        m_data.emplace_back(std::move(line));
    }
    closeFile();
    m_openQ = false;
}
