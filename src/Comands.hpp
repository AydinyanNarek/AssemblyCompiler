//
//  Commands.hpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#pragma once
#include "BaseCommands.hpp"
#include <iostream>
#include "InterpretatorUtilities.hpp"


/**
 * @brief Class CommandAssign inhereted from BaseCommand
 * Overrides the function execute()
 **/
class CommandAssign final : public BaseCommand {
public:
    CommandAssign(const std::size_t reg_id, const int value) : m_reg_id(reg_id), m_value(value) {}

    ~CommandAssign() {}

    void execute(VitualCPU &cpu) const override {
        cpu.setRegValue(m_reg_id, m_value);
    }

private:
    const std::size_t m_reg_id;
    const int m_value;
};

/**
 * @brief Class CommandPrint inhereted from BaseCommand
 * Overrides the function execute()
 **/
class CommandPrint final : public BaseCommand {
public:
    CommandPrint(const std::size_t reg_id) : m_reg_id(reg_id) {}

    ~CommandPrint() {}

    void execute(VitualCPU &cpu) const override {
        std::cout << cpu.getRegValue(m_reg_id) << std::endl;
    }

private:
    const std::size_t m_reg_id;
};

/**
 * @brief Class CommandCmp inhereted from ArithmeticComands
 * Overrides the function execute()
 **/
class CommandCmp final : public ArithmeticComands {
public:
    CommandCmp(const std::size_t left, const std::size_t right) : ArithmeticComands{left, right} {}

    ~CommandCmp() {}

    void execute(VitualCPU &cpu) const override {
        cpu.cmp(m_left, m_right);
    }
};

/**
 * @brief Class CommandCmp inhereted from JumpCommands
 * Overrides the function execute()
 **/
class CommandJump final : public JumpCommands {
public:
    CommandJump(const std::size_t new_ip = 0) : JumpCommands{new_ip - 1} {}

    ~CommandJump() {}

    void execute(VitualCPU &cpu) const override {
        cpu.setRegip(m_new_ip);
    }
};

/**
 * @brief Class CommandJz inhereted from JumpCommands
 * Overrides the function execute()
 **/
class CommandJz final : public JumpCommands {
public:
    explicit CommandJz(const std::size_t new_ip = 0) : JumpCommands{new_ip - 1} {}

    ~CommandJz() {}

    void execute(VitualCPU &cpu) const override {
      if (cpu.regStatus.ZF) {
          cpu.setRegip(m_new_ip);
      }
    }
};

/**
 * @brief Class CommandJnz inhereted from JumpCommands
 * Overrides the function execute()
 **/
class CommandJnz final : public JumpCommands {
public:
    CommandJnz(const std::size_t new_ip = 0) : JumpCommands{new_ip - 1} {}

    ~CommandJnz() {}

    void execute(VitualCPU &cpu) const override {
        if (!cpu.regStatus.ZF) {
            cpu.setRegip(m_new_ip);
        }
    }
};

/**
 * @brief Class CommandJo inhereted from JumpCommands
 * Overrides the function execute()
 **/
class CommandJo final : public JumpCommands {
public:
    CommandJo(const std::size_t new_ip = 0) : JumpCommands{new_ip - 1} {}

    ~CommandJo() {}

    void execute(VitualCPU &cpu) const override {
        if (cpu.regStatus.OF) {
            cpu.setRegip(m_new_ip);
        }
    }
};

/**
 * @brief Class CommandJno inhereted from JumpCommands
 * Overrides the function execute()
 **/
class CommandJno final : public JumpCommands {
public:
    CommandJno(const std::size_t new_ip = 0) : JumpCommands{new_ip - 1} {}

    ~CommandJno() {}

    void execute(VitualCPU &cpu) const override {
        if (!cpu.regStatus.OF) {
            cpu.setRegip(m_new_ip);
        }
    }
};

/**
 * @brief Class CommandJno inhereted from JumpCommands
 * Overrides the function execute()
 **/
class CommandJc final : public JumpCommands {
public:
    CommandJc(const std::size_t new_ip = 0) : JumpCommands(new_ip - 1) {}

    ~CommandJc() {}

    void execute(VitualCPU &cpu) const override {
        if (cpu.regStatus.CF) {
            cpu.setRegip(m_new_ip);
        }
    }
};

/**
 * @brief Class CommandJnc inhereted from JumpCommands
 * Overrides the function execute()
 **/
class CommandJnc final : public JumpCommands {
public:
    CommandJnc(const std::size_t new_ip = 0 ) : JumpCommands{new_ip - 1} {}

    ~CommandJnc() {}

    void execute(VitualCPU &cpu) const override {
        if (!cpu.regStatus.CF) {
            cpu.setRegip(m_new_ip);
        }
    }
};

/**
 * @brief Class CommandAdd inhereted from ArithmeticComands
 * Overrides the function execute()
 **/
class CommandAdd final : public ArithmeticComands {
public:
    CommandAdd(const std::size_t left, const std::size_t right) : ArithmeticComands{left, right} {}

    ~CommandAdd() {}

    void execute(VitualCPU &cpu) const override {
        cpu.add(m_left, m_right);
    }
};

/**
 * @brief Class CommandSub inhereted from ArithmeticComands
 * Overrides the function execute()
 **/
class CommandSub final : public ArithmeticComands {
public:
    CommandSub(const std::size_t left, const std::size_t right) : ArithmeticComands{left, right} {}

    ~CommandSub() {}

    void execute(VitualCPU &cpu) const override {
        cpu.sub(m_left, m_right);
    }
};

/**
 * @brief Class CommandMul inhereted from ArithmeticComands
 * Overrides the function execute()
 **/
class CommandMul final : public ArithmeticComands {
public:
    CommandMul(const std::size_t left, const std::size_t right) : ArithmeticComands{left, right} {}

    ~CommandMul() {}

    void execute(VitualCPU &cpu) const override {
        cpu.mul(m_left, m_right);
    }
};

/**
 * @brief Class CommandDiv inhereted from ArithmeticComands
 * Overrides the function execute()
 **/
class CommandDiv final : public ArithmeticComands {
public:
    CommandDiv(const std::size_t left, const std::size_t right) : ArithmeticComands{left, right} {}

    ~CommandDiv() {}

    void execute(VitualCPU &cpu) const override {
        cpu.div(m_left, m_right);
    }
};

/**
 * @brief Class CommandMov inhereted from BaseCommand
 * Overrides the function execute()
 **/
class CommandMov final : public BaseCommand {
public:
    CommandMov(const std::size_t left, const std::size_t right) : m_left(left), m_right(right) {}

    ~CommandMov() {}

    void execute(VitualCPU &cpu) const override {
        cpu.mov(m_left, m_right);
    }

private:
    const std::size_t m_left;
    const std::size_t m_right;
};

/**
 * @brief Class CommandLabel inhereted from BaseCommand
 * Overrides the function execute()
 **/
class CommandLabel final : public BaseCommand
{
public:
    CommandLabel() = default;

    ~CommandLabel() {};

    void execute([[maybe_unused]] VitualCPU& cpu) const override {}
};

/**
 * @brief Class CommandCall inhereted from JumpCommands
 * Overrides the function execute()
 **/
class CommandCall final : public JumpCommands
{
public:
    CommandCall(std::size_t new_ip = 0) : JumpCommands{new_ip} {}

    ~CommandCall() {};

    void execute(VitualCPU& cpu) const override {
        cpu.pushStack(Utils::SizeOfAssemblerTypes::W, cpu.getRegip());
        cpu.setRegip(m_new_ip);
    }
};

/**
 * @brief Class CommandRet inhereted from JumpCommands
 * Overrides the function execute()
 **/
class CommandRet final : public JumpCommands
{
public:
    CommandRet(size_t new_ip = 0) : JumpCommands{new_ip} {};

    ~CommandRet() {}

    void execute(VitualCPU& cpu) const override {
        cpu.setRegip(cpu.popStack(Utils::SizeOfAssemblerTypes::W));
    }
};

/**
 * @brief Class CommandEnd inhereted from BaseCommand
 * Overrides the function execute()
 **/
class CommandEnd final : public BaseCommand
{
public:
    CommandEnd() = default;

    ~CommandEnd() {}

    void execute(VitualCPU& cpu) const override {
        cpu.setRegip(Utils::max_int);
    }
};

/**
 * @brief Class CommandPush inhereted from BaseCommand
 * Overrides the function execute()
 **/
class CommandPush final : public BaseCommand
{
public:
    CommandPush(Utils::SizeOfAssemblerTypes::RegisterSize reg_size, std::size_t reg_id) :
     m_reg_size(reg_size), m_reg_id(reg_id) {}

    ~CommandPush() {}

    void execute(VitualCPU& cpu) const override {
        cpu.pushStack(m_reg_size, cpu.getRegValue(m_reg_id));
    }

private:
    const Utils:: SizeOfAssemblerTypes::RegisterSize m_reg_size;
    const std::size_t m_reg_id;
};

/**
 * @brief Class CommandPop inhereted from BaseCommand
 * Overrides the function execute()
 **/
class CommandPop final : public BaseCommand
{
public:
    CommandPop(Utils::SizeOfAssemblerTypes::RegisterSize reg_size, std::size_t reg_id) :
        m_reg_size(reg_size), m_reg_id(reg_id) {}

    ~CommandPop() {}

    void execute(VitualCPU& cpu) const override {
        cpu.setRegValue(m_reg_id, cpu.popStack(m_reg_size));
    }

private:
    const Utils::SizeOfAssemblerTypes::RegisterSize m_reg_size;
    const std::size_t m_reg_id;
};
