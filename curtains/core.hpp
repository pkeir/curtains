#ifndef __CORE_HPP__
#define __CORE_HPP__

#include <type_traits>
//#include "compatibility.hpp"

namespace curtains::impl {

  template <class T>
  struct id_c { using type = T; };

  template <class T>
  using id_t = typename id_c<T>::type;

  template <class, template <class...> class, class...>
  struct invoke_m {};

  template <template <class...> class M, class ...Ts>
  struct invoke_m<std::void_t<M<Ts...>>,M,Ts...> : id_c<M<Ts...>> {};

  template <class F, class ...Ts>
  using invoke = typename F::template m_invoke<Ts...>;

} // namespace curtains::impl

namespace curtains {

  template <template <class...> class M>
  struct quote {
    template <class ...Us>
    using m_invoke = typename impl::invoke_m<void,M,Us...>::type;
  };

  template <template <class...> class M>
  struct quote_c {
    template <class...Ts>
    using m_invoke = typename impl::invoke_m<void,M,Ts...>::type::type;
//  using m_invoke = typename M<Ts...>::type; // Clang needs above long form
  };

} // namespace curtains

namespace curtains::impl {

  template <class F, class ...Ts>
  struct curry {
    template <class ...Us>
    using m_invoke = invoke<F,Ts...,Us...>;
  };

  template <class, class Z, class ...>
  struct ifoldl_c              : id_c<Z> {};

  template <class F, class Z, class T, class ...Ts>
  struct ifoldl_c<F,Z,T,Ts...> : ifoldl_c<F,invoke<F,Z,T>,Ts...> {};

} // namespace curtains::impl

namespace curtains::impl::v {

  using ifoldl   = quote_c<ifoldl_c>;

} // namespace curtains::impl::v

#endif // __CORE_HPP__
