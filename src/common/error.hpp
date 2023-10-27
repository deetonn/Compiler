
#ifndef _ERROR_HPP

#include "common.hpp"

#include <format>
#include <string>

// COMPILER_API_BEGIN

class error {
private:
    std::string m_error;
public:
    template<typename ...Ts>
    inline explicit error(std::format_string<Ts...> fmt, Ts&&... args) noexcept 
        : m_error(std::format(fmt, std::forward<Ts>(args)...))
    {}

    inline const char* what() const noexcept {
        return m_error.c_str();
    }
};

// COMPILER_API_END

#define _ERROR_HPP
#endif
