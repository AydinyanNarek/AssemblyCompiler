//
//  Input.hpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#ifndef __INPUT__
#define __INPUT__

#include <vector>
#include <string>
#include <fstream>
#include <string_view>
#include "CompilerUtilities.hpp"

/**
*   @brief Class Input with all functionality 
**/
class Input {
  public:
    /**
     *   @brief Parametrized constructor of Input class
     *   @param[in] file_name - input file
    **/
    explicit Input(std::string_view file_name);

    /**
     *   @brief Destructor of Input class
    **/
    ~Input();

    /**
     *   @brief Get data from file already stored in std::vector of std::string
     *   @param[in] file_name - input file
    **/
    const std::vector<std::string>& getData() const { return m_data; }
    
    /**
     *   @brief Set data from file to class member std::vector of std::string
     *   @param[in] data - input dat to be set in std::vector
    **/
    void setData(const std::vector<std::string>& data) {this->m_data = data;}

    /**
     *   @brief      Reads data from file and stors in std::vector
    **/
    void readDataFromFile();

private:
    /**
     *   @brief Close already opend stream
    **/
    void closeFile() { (m_buffer).close();}

private:
    std::vector<std::string> m_data;
    std::fstream m_buffer{nullptr};
    std::string m_filename;
    bool m_openQ;
};

#endif //__INPUT__