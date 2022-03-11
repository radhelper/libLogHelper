//
// Created by fernando on 07/03/2022.
//

#ifndef LIB_LOGHELPER_COMMON_HPP
#define LIB_LOGHELPER_COMMON_HPP

#include <string>

namespace log_helper {
    inline std::string _exception_info(const std::string &message, const std::string &file, int line) {
#ifdef DEBUG
        return message + " - FILE:" + file + ":" + std::to_string(line) + "\n";
#else
        return "";
#endif
    }

#define EXCEPTION_MESSAGE(message) std::cerr << _exception_info(message, __FILE__, __LINE__)
#define DEBUG_MESSAGE(message) std::cout << _exception_info(message, __FILE__, __LINE__)
#define THROW_EXCEPTION(message) std::throw_with_nested(std::runtime_error(_exception_info(message, __FILE__, __LINE__)))

    enum class LoggingType {
        LOCAL_ONLY = 0,
        UDP_ONLY,
        LOCAL_AND_UDP
    };

#if LOGGING_TYPE == 0
#define LOGGING_TYPE_CLASS LoggingType::LOCAL_ONLY
#elif  LOGGING_TYPE == 1
#define LOGGING_TYPE_CLASS LoggingType::UDP_ONLY
#elif  LOGGING_TYPE == 2
#define LOGGING_TYPE_CLASS LoggingType::LOCAL_AND_UDP
#endif

}
#endif //LIB_LOGHELPER_COMMON_HPP
