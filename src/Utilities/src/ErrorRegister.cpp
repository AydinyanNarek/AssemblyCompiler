#include "../include/ErrorManager/ErrorRegister.h"

namespace Errors
{

	auto ErrorRegister::CoreErrors() -> ErrorRegister::ErrorContainer & {
		static ErrorContainer errmap = {
			{"NoError", "No errors occurred."},
		};
		return errmap;
	}

	void ErrorRegister::registerErrors(const std::vector<ErrorData> &errors) {
		for (auto &&err : errors) {
			setErrors(err);
		}
	}

	void ErrorRegister::setErrors(const ErrorData &errorData) {
		auto &errorMap = CoreErrors();
		auto elem = errorMap.emplace(errorData.first, errorData.second);
		if (!elem.second) {
			errorMap.emplace(errorData.first, errorData.first);
		}
	}

	const std::string ErrorRegister::getError(const std::string &errorName, [[maybe_unused]] const std::string &errorDescription) {
		const auto &exception = CoreErrors().find(errorName);
		if (exception == CoreErrors().end()) {
			throw("Unregistered Error.");
		}
		return exception->second + "   " + errorDescription;
	}
} // namespace Errors