#ifndef __CONST_HPP__
#define __CONST_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class T, class>        struct const_c  : id_c<T> {};
  template <class T, class>        struct const2_c : id_c<T> {};
  template <class T, class, class> struct const3_c : id_c<T> {};
  template <class T, class...>     struct constv_c : id_c<T> {};

} //  namespace curtains::impl

namespace curtains::v {

  using const_  = quote_c<impl::const_c>;
  using const2  = quote_c<impl::const_c>;
  using const3  = quote_c<impl::const3_c>;
  using constv  = quote_c<impl::constv_c>;

} // namespace curtains::v

namespace curtains::n {

  using const_  = bases<v::const_,ic<2>>;
  using const2  = bases<v::const_,ic<2>>;
  using const3  = bases<v::const3,ic<3>>;
  using constv3 = bases<v::constv,ic<3>>;

} // namespace curtains::n

#endif // __CONST_HPP__
