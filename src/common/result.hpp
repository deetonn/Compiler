#ifndef _RESULT_HPP

#include "common.hpp"

#include <variant>
#include <type_traits>

// COMPILER_API_BEGIN

template<class T, class E>
class result {
private:
  std::variant<T, E> m_variant;
public:
  static_assert(std::is_move_assignable<T>::value, "result<T>: value must be move assignable.");
  static_assert(std::is_move_assignable<E>::value, "result<E>: error must be move assignable.");
  
  result() = delete;
  inline result(T&& ok)
    : m_variant(std::move(ok))
  {}
  inline result(E&& err)
    : m_variant(std::move(err))
  {}

  inline bool is_err() const noexcept {
    return std::get<1>(&m_variant) != nullptr;  
  }

  inline bool is_okay() const noexcept {
    return is_err() == false;  
  }

  inline T* get() const noexcept {
    return std::get<0>(&m_variant); 
  }

  inline E* get_err() const noexcept {
    return std::get<1>(&m_variant);
  }
};

// COMPILER_API_END

#define _RESULT_HPP
#endif // !_RESULT_HPP
