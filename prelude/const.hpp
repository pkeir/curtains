#ifndef __CONST_HPP__
#define __CONST_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class T, class>
  struct const_c : id_c<T> {};
  template <class T, class>
  struct const2_c : id_c<T> {};
  template <class T, class, class>
  struct const3_c : id_c<T> {};
  template <class T, class...>
  struct constv_c : id_c<T> {};

  template <class T, class U>
  using const_t  = typename const2_c<T,U>::type;
  template <class T, class U>
  using const2_t = typename const2_c<T,U>::type;
  template <class T, class U, class V>
  using const3_t = typename const3_c<T,U,V>::type;
  template <class T, class ...Ts>
  using constv_t = typename constv_c<T,Ts...>::type;

} //  namespace curtains::impl

namespace curtains::v {

  using const_  = quote<impl::const_t>;
  using const2  = quote<impl::const_t>;
  using const3  = quote<impl::const3_t>;
  using constv  = quote<impl::constv_t>;

} // namespace curtains::v

namespace curtains::n {

  using const_  = bases<v::const_,ic<2>>;
  using const2  = bases<v::const_,ic<2>>;
  using const3  = bases<v::const3,ic<3>>;
  using constv3 = bases<v::constv,ic<3>>;

} // namespace curtains::n

#endif // __CONST_HPP__
