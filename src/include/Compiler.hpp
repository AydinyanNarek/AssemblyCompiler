//
//  Compiler.hpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#ifndef __COMPILER__
#define __COMPILER__

#include "BaseCommands.hpp"
#include "Comands.hpp"
#include "InterpretatorUtilities.hpp"
#include <string>
#include <memory>
#include <forward_list>

/**
 * @brief Class Compiler with all functionality
 **/
class Compiler
{
public:
    /**
     * @brief Registers all project errors
    **/
    static void registerErrors();    
private:
    /**
     * @brief Enum Class Command with command set
     **/
    enum class Command
    {
        assign,
        print,
        cmp,
        jump,
        jz,
        jnz,
        jo,
        jno,
        jc,
        jnc,
        add,
        sub,
        mul,
        div,
        mov,
        call,
        ret,
        end,
        push,
        pop
    };

private:
    std::unordered_map<std::string, std::forward_list<JumpCommands*>> nameInCommands;

    std::unordered_map<CommandLabel*, std::string> nameInComandLable;
	const std::unordered_map<std::string, Command> s_string_enum_map;
    std::unordered_map<std::string, Command> assemblerCommandsMap;

private:
    /**
     *   @brief Splits the string with given algorithm
     *   @param[in] str - input string
     *   @param[out] vector - output vector of strings
    **/
    std::vector<std::string> split(const std::string& str);

    /**
     *   @brief Makes print command
     *   @param[in] id - register ID
     *   @param[in] arg - for tuture implementation
     *   @param[out] pointer - smart pointer with type BaseCommand
    **/
    std::unique_ptr<BaseCommand> printCommandMake(const std::size_t id, [[maybe_unused]]const std::size_t arg);

    /**
     *   @brief Makes command assign
     *   @param[in] id - register ID
     *   @param[in] value - input value to be prented
     *   @param[out] pointer - smart pointer with type BaseCommand
    **/
    std::unique_ptr<BaseCommand> assigneCommandMake(const std::string &id, const std::string &value);

    /**
     *   @brief Make jumps
     *   @param[in] program - referance of std::vector with type smart pointers - with type BaseCommand
    **/
    void make_jamps(std::vector<std::unique_ptr<BaseCommand>>& program);

    /**
     *   @brief Makes command jump
     *   @param[in] label_name - input value std::string for lables
     *   @param[in] arg - variatic template //for future 
     *   @param[out] pointer - smart pointer with type BaseCommand
    **/
    template <class T, class ... TArgs>
    std::unique_ptr<BaseCommand> jumpCommandMake(const std::string& label_name, [[maybe_unused]]TArgs&... arg);

    /**
     *   @brief Template function for Makeing command ls
     *   @param[in] reg_size - input value std::string for register size
     *   @param[in] reg_id - input value std::string for register id
     *   @param[out] pointer - smart pointer with type BaseCommand
    **/
    template <class T>
    std::unique_ptr<BaseCommand> lsCommandMake(const std::string& reg_size, const std::string& reg_id);

    /**
     *   @brief Get an incoming isntruction 
     *   @param[in] command - input value std::string command to be interpreted
     *   @param[in] arg - variatic template //for future 
     *   @param[out] pointer - smart pointer with type BaseCommand
    **/
    template <class... TArgs>
    std::unique_ptr<BaseCommand> get_instruction(const std::string& command, TArgs &&... args); 

public:
    /**
     *   @brief Get an incoming isntruction 
     *   @param[in] vec - input value std::vector of strings to be parsed to binary code
     *   @param[out] vector - std::vector of smart pointers to BaseCommand class in binary mode
    **/
    std::vector<std::unique_ptr<BaseCommand>>createCode(std::vector<std::string>& vec);

    /**
     *   @brief Constructor of Compiler class
     *   @param[in] file_name - input file
    **/
    Compiler();

    /**
     *   @brief Default destructor of Compiler class
     *   @param[in] file_name - input file
    **/
    ~Compiler() = default;
};

template <class T, class ... TArgs>
std::unique_ptr<BaseCommand> Compiler::jumpCommandMake(const std::string& label_name, [[maybe_unused]]TArgs&... arg) {
    auto inst = std::make_unique<T>();
    nameInCommands[label_name].push_front(inst.get());
    return inst;
}

template <class T>
std::unique_ptr<BaseCommand> Compiler::lsCommandMake(const std::string& reg_size, const std::string& reg_id) {
    return std::make_unique<T>(Utils::SizeOfAssemblerTypes::get_reg_size(reg_size), Utils::toIntt(reg_id));
}

template <class... TArgs>
    std::unique_ptr<BaseCommand> Compiler::get_instruction(const std::string& command, TArgs &&... args) {

        const auto inputcommand = assemblerCommandsMap.find(command);
        if (inputcommand == assemblerCommandsMap.end()) {
            auto instruction = std::make_unique<CommandLabel>();
            nameInComandLable[instruction.get()] = command;
            return instruction;
        }

        switch ((*inputcommand).second) {
            case Command::assign: {
                return assigneCommandMake(args...);
            }
            case Command::print: {
                return printCommandMake(Utils::toSTDSize_t(args)...);
            }
            case Command::sub: {
                return std::make_unique<CommandSub>(Utils::toSTDSize_t(args)...);
            }
            case Command::mul: {
                return std::make_unique<CommandMul>(Utils::toSTDSize_t(args)...);
            }
            case Command::div: {
                return std::make_unique<CommandDiv>(Utils::toSTDSize_t(args)...);
            }
            case Command::mov: {
                return std::make_unique<CommandMov>(Utils::toSTDSize_t(args)...);
            }
            case Command::cmp: {
                return std::make_unique<CommandCmp>(Utils::toSTDSize_t(args)...);
            }
            case Command::jump: {
                return jumpCommandMake<CommandJump>(args...);
            }
            case Command::jz: {
                return jumpCommandMake<CommandJz>(args...);
            }
            case Command::jnz: {
                return jumpCommandMake<CommandJnz>(args...);
            }
            case Command::jo: {
                return jumpCommandMake<CommandJo>(args...);
            }
            case Command::jno: {
                return jumpCommandMake<CommandJno>(args...);
            }
            case Command::jc: {
                return jumpCommandMake<CommandJc>(args...);
            }
            case Command::jnc: {
                return jumpCommandMake<CommandJnc>(args...);
            }
            case Command::add: {
                return std::make_unique<CommandAdd>(Utils::toSTDSize_t(args)...);
            }
            case Command::ret: {
                 return std::make_unique<CommandRet>();
            }
            case Command::end: {
               return std::make_unique<CommandEnd>();
            }
            case Command::push: {
                return lsCommandMake<CommandPush>(args...);
            }
            case Command::pop: {
                return lsCommandMake<CommandPop>(args...);
            }
            case Command::call: {
                return jumpCommandMake<CommandCall>(args...);
            }
            default: {
                throw("Unregistered Command.");
            }
        }
        return {};
    }

#endif // __COMPILER__