//
//  BaseCommand.hpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//

#pragma once

#include "VirtualCpu.hpp"

/**
 * @brief Base class for all commands 
 * The child classes must override the function execute()
 **/
class BaseCommand {
public:
    BaseCommand() = default;
    virtual void execute(VitualCPU &cpu) const = 0;

    virtual ~BaseCommand() {};
};

/**
 * @brief Base class for all arithmetics commands, inhereted from BaseCommand
 * The child classes must override the function execute()
 **/
class ArithmeticComands : public BaseCommand {
public:
    ArithmeticComands() = delete;
    ArithmeticComands(const ArithmeticComands&) = delete;
    ArithmeticComands(const std::size_t lOperand, const std::size_t rOperand) : m_left(lOperand), m_right(rOperand) {};
    virtual ~ArithmeticComands() {}

protected:
    const std::size_t m_left;
    const std::size_t m_right;
}; 

/**
 * @brief Base class for all jump commands, inhereted from BaseCommand
 * The child classes must override the function execute()
 **/
class JumpCommands : public BaseCommand {
public:
    JumpCommands() = delete;
    JumpCommands(const JumpCommands&) = delete;
    virtual ~JumpCommands() {}
    
    JumpCommands(const std::size_t new_ip) : m_new_ip(new_ip) {};
    void set_ip(const size_t ip) {
      m_new_ip = ip;
    }
protected:
    std::size_t m_new_ip;
};

//#endif //#endif// 
