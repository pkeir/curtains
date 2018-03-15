#ifndef __CORE_HPP__
#define __CORE_HPP__

#include "compatibility.hpp"

namespace curtains::impl {

  template <class T>
  struct id_c { using type = T; };

  template <class T>
  using id_t = typename id_c<T>::type;

  template <class, template <class...> class, class...>
  struct invoke_m {};

  template <template <class...> class M, class ...Ts>
  struct invoke_m<void_t<M<Ts...>>,M,Ts...> : id_c<M<Ts...>> {};

  template <class F, class ...Ts>
  using invoke = typename F::template m_invoke<Ts...>;

} // namespace curtains::impl

namespace curtains {

  template <template <class...> class M>
  struct quote {
    template <class ...Us>
    using m_invoke = typename impl::invoke_m<void,M,Us...>::type;
  };

} // namespace curtains

namespace curtains::impl {

  template <class F, class ...Ts>
  struct curry {
    template <class ...Us>
    using m_invoke = invoke<F,Ts...,Us...>;
  };

  template <class, class Z, class ...>
  struct ifoldl                         : id_c<Z> {};

  template <class F, class Z, class T, class ...Ts>
  struct ifoldl<F,Z,T,Ts...>            : ifoldl<F,invoke<F,Z,T>,Ts...> {};

  template <class F, class Z, class ...Ts>
  using ifoldl_t = typename ifoldl<F,Z,Ts...>::type;

} // namespace curtains::impl

namespace curtains::impl::v {

  using ifoldl   = quote<ifoldl_t>;

} // namespace curtains::impl::v

#endif // __CORE_HPP__
