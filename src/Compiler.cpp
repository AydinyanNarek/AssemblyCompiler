//
//  Compiler.cpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#include "Compiler.hpp"

Compiler::Compiler() : nameInCommands(0), 
    nameInComandLable(0),
    s_string_enum_map(0), 
    assemblerCommandsMap({
        {"ASSIGN", Command::assign},
        {"PRINT", Command::print},
        {"CMP", Command::cmp},
        {"JNC", Command::jnc},
        {"ADD", Command::add},
        {"SUB", Command::sub},
        {"MUL", Command::mul},
        {"JUMP", Command::jump},
        {"JZ", Command::jz},
        {"JNZ", Command::jnz},
        {"JO", Command::jo},
        {"JNO", Command::jno},
        {"JC", Command::jc},
        {"RET", Command::ret},
        {"END", Command::end},
        {"PUSH", Command::push},
        {"POP", Command::pop},
        {"DIV", Command::div},
        {"MOV", Command::mov},
        {"CALL", Command::call}}) {};

std::vector<std::string> Compiler::split(const std::string& str) {

    std::istringstream iss(str);
    std::vector<std::string> cont;
    std::copy(std::istream_iterator<std::string>(iss),
    std::istream_iterator<std::string>(),
    std::back_inserter(cont));
    return cont;  
}

std::unique_ptr<BaseCommand> Compiler::printCommandMake(const std::size_t id, [[maybe_unused]]const std::size_t arg) {
    return std::unique_ptr<BaseCommand>(std::make_unique<CommandPrint>(id));
}

std::unique_ptr<BaseCommand> Compiler::assigneCommandMake(const std::string &id, const std::string &value) {
    return std::unique_ptr<BaseCommand>(std::make_unique<CommandAssign>(Utils::toSTDSize_t(id), Utils::toIntt(value)));
}

void Compiler::make_jamps(std::vector<std::unique_ptr<BaseCommand>>& program) {
    for (auto i = 0; i < program.size(); ++i)
    {
        auto label_to_name_pair = nameInComandLable.find(static_cast<CommandLabel*>(program[i].get()));

        if (label_to_name_pair == nameInComandLable.end()) {
            continue;
        }

        auto [label, name] = *label_to_name_pair;
        nameInComandLable.erase(static_cast<CommandLabel*>(program[i].get()));

        auto name_to_jumps_pair = nameInCommands.find(name);

        if (name_to_jumps_pair == nameInCommands.end())
        {
            continue;
        }

        auto [n, jumps] = *name_to_jumps_pair;
       // auto jumps = name_to_jumps_pair->second;

        for (auto ji : jumps)
        {
            ji->set_ip(i);
        }

        nameInCommands.erase(name);
    }

    if (!nameInCommands.empty() || !nameInComandLable.empty())
    {
        throw std::invalid_argument{"Invalid Jump or label"};
    }

    nameInCommands.clear();
    nameInComandLable.clear();
}

std::vector<std::unique_ptr<BaseCommand>> Compiler::createCode(std::vector<std::string>& vec) {
    std::vector<std::unique_ptr<BaseCommand>> temp;
    std::unique_ptr<BaseCommand> instruction = nullptr;

    for (int  i = 0; i < vec.size(); ++i){
        auto tokens = split(vec[i]);
        switch (tokens.size())
        {
            case 3: {
                instruction = get_instruction(tokens[0], tokens[1], tokens[2]);
                temp.emplace_back(std::move(instruction));
                break;
            }
            case 2: {
                instruction = get_instruction(tokens[0], tokens[1], "");
                temp.emplace_back(std::move(instruction));
                break;
            }
            case 1: {
                instruction = get_instruction(tokens[0], "", "");
                temp.emplace_back(std::move(instruction));
                break;
            }
            default: {
                throw("Unregistered Command.");
            }
        }
    }
    make_jamps(temp);

    return temp;

}
