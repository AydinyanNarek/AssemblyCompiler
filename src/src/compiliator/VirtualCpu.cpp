//
//  VirtualCpu.cpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#include "../../include/compiliator/VirtualCpu.hpp"
#include <cassert>
#include <cstring>
#include "../../include/compiliator/CompilerUtilities.hpp"
#include "../../Utilities/include/ErrorManager/ErrorRegister.h"

bool VitualCPU::addOperationOverflow(const std::size_t l, const std::size_t r) {
    if (cpuRegisteres[r] > 0 && cpuRegisteres[l] > Utils::max_int - cpuRegisteres[r]){
        return true;
    }
    return false;   
}

bool VitualCPU::addOperationNnderflow(const std::size_t l, const std::size_t r) {
    if (cpuRegisteres[r] < 0 && (cpuRegisteres[l] < Utils::min_int - cpuRegisteres[r])){
        return true;
    }
    return false;
}

bool VitualCPU::subOperationOverflow(const std::size_t l, const std::size_t r) {
    if (cpuRegisteres[r] < 0 && (cpuRegisteres[l] > Utils::max_int + cpuRegisteres[r])){
        return true;
    }
    return false;
}

bool VitualCPU::subOperationUnderflow(const std::size_t l, const std::size_t r) {
    if (cpuRegisteres[r] > 0 && (cpuRegisteres[l] < Utils::min_int + cpuRegisteres[r])){
        return true;
    }
    return false;
}

bool VitualCPU::mulOperationOverflow(const std::size_t l, const std::size_t r) {
    if ((cpuRegisteres[l] > Utils::max_int / cpuRegisteres[r]) || 
        ((cpuRegisteres[l] == -1) && (cpuRegisteres[r] == Utils::min_int))){
         return true;
     }
     return false;
}

bool VitualCPU::mulOperationUnderflow(const std::size_t l, const std::size_t r) {
    if (cpuRegisteres[l] < Utils::min_int / cpuRegisteres[r] || 
        ((cpuRegisteres[l] == -1) && (cpuRegisteres[r] == Utils::min_int))){
        return true;
    }
    return false;
}

void VitualCPU::add(const std::size_t l, const std::size_t r) {
    regStatus.OF = addOperationOverflow(l, r) || addOperationNnderflow(l, r);
    cpuRegisteres[l] += cpuRegisteres[r];
    regStatus.ZF = (cpuRegisteres[l] == 0);
    regStatus.CF = ((cpuRegisteres[l] >> 31) & 1u);
}

void VitualCPU::sub(const std::size_t l, const std::size_t r) {
    regStatus.OF = subOperationOverflow(l, r) || subOperationUnderflow(l, r);
    cpuRegisteres[l] -= cpuRegisteres[r];
    regStatus.ZF = (cpuRegisteres[l] == 0);
    regStatus.CF = ((cpuRegisteres[l] >> 31) & 1u);
}

void VitualCPU::mul(const std::size_t l, const std::size_t r) {
    regStatus.OF = mulOperationOverflow(l, r) || mulOperationUnderflow(l, r);
    cpuRegisteres[l] *= cpuRegisteres[r];
    regStatus.ZF = (cpuRegisteres[l] == 0);
    regStatus.CF = ((cpuRegisteres[l] >> 31) & 1u);
}

void VitualCPU::div(const std::size_t l, const std::size_t r) {
    regStatus.OF = false;
    cpuRegisteres[l] /= cpuRegisteres[r];
    regStatus.ZF = (cpuRegisteres[l] == 0);
    regStatus.CF = ((cpuRegisteres[l] >> 31) & 1u);
}

void VitualCPU::cmp(const std::size_t l, const std::size_t r) {
    regStatus.OF = subOperationOverflow(l, r) || subOperationUnderflow(l, r);
    auto temp = cpuRegisteres[l] - cpuRegisteres[r];
    regStatus.ZF = (temp == 0);
    regStatus.CF = ((temp >> 31) & 1u);
}

void VitualCPU::mov(const std::size_t l, const std::size_t r) {
    cpuRegisteres[l] = cpuRegisteres[r];
}

int VitualCPU::getRegValue(const std::size_t id) {
    return cpuRegisteres[id];
}

void VitualCPU::setRegValue(const std::size_t id, int value) {
    cpuRegisteres[id] = value;
}

void VitualCPU::setRegip(const std::size_t new_ip)
{
    m_ip = new_ip;
}

void VitualCPU::pushStack(const Utils::SizeOfAssemblerTypes::RegisterSize size, const int64_t value) {
    switch (size)
    {
        case Utils::SizeOfAssemblerTypes::RegisterSize::B:{
            LoadToStack(static_cast<int8_t>(value), this);
            break;
        }
        case Utils::SizeOfAssemblerTypes::RegisterSize::W:{
            LoadToStack(static_cast<int16_t>(value), this);
            break;
        }
        case Utils::SizeOfAssemblerTypes::RegisterSize::DB:{
            LoadToStack(static_cast<int32_t>(value), this);
            break;
        }
        case Utils::SizeOfAssemblerTypes::RegisterSize::DW:{
            LoadToStack(static_cast<int64_t>(value), this);
            break;
        }
        default:
            Errors::ErrorRegister::Throw("UnregisteredTypeError");
    }
}

int64_t VitualCPU::popStack(const Utils::SizeOfAssemblerTypes::RegisterSize size) {
    switch (size) {
        case Utils::SizeOfAssemblerTypes::RegisterSize::B: {
            int8_t tmp = 0;
            std::memcpy(&tmp, m_stack_pointer, sizeof(int8_t));
            m_stack_pointer += sizeof(int8_t);
            return tmp;
        }
        case Utils::SizeOfAssemblerTypes::RegisterSize::W: {
            int16_t tmp = 0;
            std::memcpy(&tmp, m_stack_pointer, sizeof(int16_t));
            m_stack_pointer += sizeof(int16_t);
            return tmp;
        }
        case Utils::SizeOfAssemblerTypes::RegisterSize::DB: {
            int32_t tmp = 0;
            std::memcpy(&tmp, m_stack_pointer, sizeof(int32_t));
            m_stack_pointer += sizeof(int32_t);
            return tmp;
        }
        case Utils::SizeOfAssemblerTypes::RegisterSize::DW: {
            int64_t tmp = 0;
            std::memcpy(&tmp, m_stack_pointer, sizeof(int64_t));
            m_stack_pointer += sizeof(int64_t);
            return tmp;
        }
        default: {
            Errors::ErrorRegister::Throw("UnregisteredTypeError");
        }
    }
}

