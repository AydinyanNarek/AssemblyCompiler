/**
 * @file    ErrorRegister.h
 * @author  Narek Aydinyan <aydinyan.narek@gmail.com>
 * @date    7/20/2019
 * @brief   Defination of class ErrorRegister with exception handling
 **/
#ifndef __ERROR_REGISTER__
#define __ERROR_REGISTER__

#include <vector>
#include <unordered_map>
#include <utility>

namespace Errors
{

    /**
	 * @class	ErrorManager
	 * @brief	class Agregation for error handling
	 *
     * ErrorRegister class contains unorderd_map as static member for registering and throwing exeptions.
     * This class is agregation type and for not allowing to create object of this class all constructors was deleted.
	 **/
    class ErrorRegister {
    private:
        /// For registering error message
        ///The form is {error name, error description}
        using ErrorData = std::pair<std::string, std::string>;

        /// Errors are stored in a unorderd_map.
        ///The form is {error name, error description}
        using ErrorContainer = std::unordered_map<std::string, std::string>;

    public:
        /**
        * @brief Default constructor of ErrorRegister class
        **/
        ErrorRegister() = delete;

        /**
        * @brief Coppy constructor of ErrorRegister class
        **/
        ErrorRegister(const ErrorRegister &) = delete;

        /**
        * @brief Move constructor of ErrorRegister class
        **/
        ErrorRegister(ErrorRegister &&) = delete;

        /**
        * @brief Parametrized constructor of ErrorRegister class
        **/
        template <typename T>
        ErrorRegister(std::initializer_list<T>) = delete;

        /**
        * @brief 	Function for registering error messages
        * @param 	errors - std::vector of std::pair s: {"ErrorName", "Error description"}
        **/
        static void registerErrors(const std::vector<ErrorData> &errors);

        /**
        * @brief 	Function for throwing exception
        * @param 	T - error name with type of std::string
        * @param 	T1 - template parameter pack for description. It can exist, not exist and contain 1 element
        **/
        template <typename T = const std::string &, typename... T1>
        [[noreturn]] static void Throw(T, T1 &&... args);

    private:
        /**
        * @brief 	Function for adding new error in static member unorderd_map
        * @param 	newError -  std::pair of strings  {error name, error description}
        **/
        static void setErrors(const ErrorData &newError);

        /**
        * @brief 	Function for gettin error from unorderd_map if exist
        * @param 	errorDescription -  error description which may not exist
        **/
        static const std::string getError(const std::string &errorName, [[maybe_unused]] const std::string &errorDescription);

        /**
         * @brief 	Returns referance of registered errors
        **/
        static ErrorContainer &CoreErrors();
    };

    template <typename T, typename... T1>
    void ErrorRegister::Throw(T errorName, [[maybe_unused]] T1 &&... args) {
        constexpr auto count = sizeof...(args);
        std::string tempString = "";
        if (count > 1)
        {
            throw("Invalid count of arguments for throwing exeption");
        }
        else if (count == 1)
        {
            tempString = std::string(T(std::forward<T1>(args)...));
        }

        auto err = getError(errorName, tempString);
        throw(err);
    }
} // namespace Errors
#endif // __ERROR_REGISTER__
