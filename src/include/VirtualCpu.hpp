//
//  VirtualCpu.hpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#ifndef __VIRTUALCPU__
#define __VIRTUALCPU__
#include "InterpretatorUtilities.hpp"
#include <cstring>

/**
 *   @brief class VitualCPU
**/
class VitualCPU {
    
private:
    /**
     *   @brief Struct statuss register
    **/
    struct StatusRegister
    {
        bool CF : 1;
        bool ZF : 1;
        bool OF : 1;
        bool ID : 1;
    };

public:
    VitualCPU() 
    :cpuRegisteres {100}
    {};

    ~VitualCPU() {}

    std::size_t m_ip{0};

    StatusRegister regStatus{};
private:
    std::size_t max_stack_size{255u};
    std::vector<int> cpuRegisteres;
    std::unique_ptr<typename std::remove_pointer<char*const>::type, void (*)(char*const)> m_call_stack{Utils::sPointerWrapper(new char[max_stack_size])};
    char *m_stack_pointer{m_call_stack.get() + max_stack_size};

public:
    /**
     *   @brief Operation add leftOperand add to rightOperand
     *   @param[in]  leftOperand - integer value size of std::size_t
     *   @param[in]  rightOperand - integer value size of std::size_t
    **/
    void add(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Operation sub leftOperand sub to rightOperand
     *   @param[in]  leftOperand - integer value size of std::size_t
     *   @param[in]  rightOperand - integer value size of std::size_t
    **/
    void sub(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Operation mul leftOperand mul to rightOperand
     *   @param[in]  leftOperand - integer value size of std::size_t
     *   @param[in]  rightOperand - integer value size of std::size_t
    **/
    void mul(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Operation div rightOperand div to leftOperand
     *   @param[in]  leftOperand - integer value size of std::size_t
     *   @param[in]  rightOperand - integer value size of std::size_t
    **/
    void div(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Operation mov rightOperand mov to leftOperand
     *   @param[in]  leftOperand - integer value size of std::size_t
     *   @param[in]  rightOperand - integer value size of std::size_t
    **/
    void mov(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Operation cmp rightOperand cmp to leftOperand
     *   @param[in]  leftOperand - integer value size of std::size_t
     *   @param[in]  rightOperand - integer value size of std::size_t
    **/
    void cmp(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Get register value
     *   @param[in]  id - register ID
     *   @param[out] integer - value of register 
    **/
    int getRegValue(const std::size_t id);

    /**
     *   @brief Get register IP
     *   @param[out] std::size_t - register IP 
    **/
    std::size_t getRegip() const { return m_ip; }

    /**
     *   @brief Set register value
     *   @param[in]  id - register ID
     *   @param[in]  value - value of register 
    **/
    void setRegValue(const std::size_t id, int value);

    /**
     *   @brief Set register Ip
     *   @param[in]  new_ip - register IP
    **/
    void setRegip(const std::size_t new_ip);

    /**
     *   @brief Push register value to stack
     *   @param[in]  type_size - size of incomeing type 
     *   @param[in]  value - value of incoming type which will be casted to type_size
    **/
    void pushStack(const Utils::SizeOfAssemblerTypes::RegisterSize type_size, const int64_t value);

    /**
     *   @brief Pop value from satck
     *   @param[in]  type_size - size of incomeing type 
    **/
    int64_t popStack(const Utils::SizeOfAssemblerTypes::RegisterSize type_size);

private:
    /**
     *   @brief Template helper function for loading value to the stack
     *   @param[in]  val - template parameter 
     *   @param[in]  ob - pointer to VitualCPU class
    **/
    template<typename T>
    void LoadToStack(const T val, VitualCPU* ob) {
        using ValueType = decltype(val);
        ValueType tmp = static_cast<ValueType>(val);
        ob->m_stack_pointer -= sizeof(ValueType);
        std::memcpy(ob->m_stack_pointer, &tmp, sizeof(ValueType));
    }

    /**
     *   @brief Checking for add operation overflow
     *   @param[in]  leftOperand - incoming argument size of std::size_t 
     *   @param[in]  rightOperand - incoming argument size of std::size_t 
     *   @param[out] bool - result true inf not overflow false in other case
    **/
    bool addOperationOverflow(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Checking for add operation overflow in negative side
     *   @param[in]  leftOperand - incoming argument size of std::size_t 
     *   @param[in]  rightOperand - incoming argument size of std::size_t 
     *   @param[out] bool - result true inf not overflow false in other case
    **/
    bool addOperationNnderflow(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Checking for sub operation overflow 
     *   @param[in]  leftOperand - incoming argument size of std::size_t 
     *   @param[in]  rightOperand - incoming argument size of std::size_t 
     *   @param[out] bool - result true inf not overflow false in other case
    **/
    bool subOperationOverflow(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Checking for sub operation overflow in negative side
     *   @param[in]  leftOperand - incoming argument size of std::size_t 
     *   @param[in]  rightOperand - incoming argument size of std::size_t 
     *   @param[out] bool - result true inf not overflow false in other case
    **/
    bool subOperationUnderflow(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Checking for mul operation overflow in 
     *   @param[in]  leftOperand - incoming argument size of std::size_t 
     *   @param[in]  rightOperand - incoming argument size of std::size_t 
     *   @param[out] bool - result true inf not overflow false in other case
    **/
    bool mulOperationOverflow(const std::size_t leftOperand, const std::size_t rightOperand);

    /**
     *   @brief Checking for mul operation overflow in negative side
     *   @param[in]  leftOperand - incoming argument size of std::size_t 
     *   @param[in]  rightOperand - incoming argument size of std::size_t 
     *   @param[out] bool - result true inf not overflow false in other case
    **/
    bool mulOperationUnderflow(const std::size_t leftOperand, const std::size_t rightOperand);
};
#endif //__VIRTUALCPU__//