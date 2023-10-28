
#include <iostream>
#ifndef _COMMON_IO_HPP

#include <format>
#include <string>
#include <iostream>

template<typename... Ts>
inline void print(const std::format_string<Ts...> fmt, Ts&&... args) noexcept {
    const auto _Fmtd = std::format(fmt, std::forward<Ts>(args)...);
    std::cout << _Fmtd;
}

template<typename... Ts>
inline void println(const std::format_string<Ts...> fmt, Ts&&... args) noexcept {
    const auto _Fmtd = std::format(fmt, std::forward<Ts>(args)...);
    std::cout << _Fmtd << '\n';
}

template<typename... Ts>
inline void eprint(const std::format_string<Ts...> fmt, Ts&&... args) noexcept {
    const auto _Fmtd = std::format(fmt, std::forward<Ts>(args)...);
    std::cerr << _Fmtd;
}

template<typename... Ts>
inline void eprintln(const std::format_string<Ts...> fmt, Ts&&... args) noexcept {
    const auto _Fmtd = std::format(fmt, std::forward<Ts>(args)...);
    std::cerr << _Fmtd << '\n';
}

#define _COMMON_IO_HPP
#endif // !_COMMON_IO_HPP