#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <type_traits>

#include "curtains/curtains_includes.hpp"

namespace curtains {

template <unsigned ...>
struct u_seq {};

template <class T>
using type = typename T::type;

template <class, class = void_t<>>
struct has_type_member                             : std::false_type {};

template <class T>
struct has_type_member<T,void_t<typename T::type>> : std::true_type {};

template <class T>
constexpr bool has_type_member_v = has_type_member<T>::value;

template <class, class, class...>
struct can_invoke : std::false_type {};

template <class F, class ...Ts>
struct can_invoke<void_t<impl::invoke<F,Ts...>>,F,Ts...> : std::true_type {};

template <class F, class ...Ts>
constexpr bool can_invoke_v = can_invoke<void,F,Ts...>::value;

// Converts a boolean value metafunction into a boolean type metafunction
template <template <class...> class F, class ...Ts>
using tt = std::enable_if<F<Ts...>::value>;

template <class T, class U>
using is_same_tt    = typename tt<std::is_same,T,U>::type;
template <class T>
using is_pointer_tt = typename tt<std::is_pointer,T>::type;

} // namespace  curtains

#endif // __UTIL_HPP__

