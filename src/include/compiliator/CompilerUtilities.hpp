//
//  InertpretatorUtilities.hpp
//
//  Created by Narek Aydinyan on 4/25/19.
//  Copyright © 2019 Narek Aydinyan. All rights reserved.
//
#ifndef __UTILITIES__
#define __UTILITIES__

#include <unordered_map>
#include <memory>
#include <vector>
#include <regex>
#include <string>
#include <algorithm>
#include <exception>
#include <iostream>
#include <cassert>

#if  defined (__GNUC__ )
    #define Static_Unused __attribute__((unused)) 
#elif defined (_MSC_VER)
    #define Static_Unused static
#elif defined (__clang__)
    #define Static_Unused static
#endif

/**
*   @brief Namespace Utils with helper functions 
**/
namespace Utils 
{
    /**
    *   @brief Wrapp pointer to c++ smart pointer with custom deleter
    **/
    template <class T, class T1 = void (*)(T)>
    auto sPointerWrapper(T object) {
        return std::unique_ptr<typename std::remove_pointer<T>::type, T1>(object, [](T object) { delete [] object;
                                                                                                object = nullptr;});
    }

    /**
    *   @brief struct SizeOfAssemblerTypes
    **/
    struct SizeOfAssemblerTypes {
        enum RegisterSize
        {
            B,
            W,
            DB,
            DW
        };

        static inline const std::unordered_map<std::string, SizeOfAssemblerTypes::RegisterSize> regSize {
            {"B", RegisterSize::B},
            {"W", RegisterSize::W},
            {"DB", RegisterSize::DB},
            {"DW", RegisterSize::DW},
        };

        static SizeOfAssemblerTypes::RegisterSize get_reg_size(const std::string &name) {
            return regSize.at(name);
        }
    };

    namespace{
        constexpr int max_int = 2147483647; // Maximum value of integer
        constexpr int min_int = -2147483648;// Minimum value of integer

        Static_Unused std::size_t toSTDSize_t(const std::string num) {
            if (num.empty()) {
                return 0;
            }
            size_t result = static_cast<std::size_t>(std::stoi(num));

            return result;
        }

        Static_Unused int toIntt(const std::string& num) {
            if (num.empty()) {
                return 0;
            }
            int result = std::stoi(num);
            return result;
        }
    }
}
#endif //__UTILITIES__//