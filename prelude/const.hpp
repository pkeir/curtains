#ifndef __CONST_HPP__
#define __CONST_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class T, class>        using const_t  = T;
  template <class T, class>        using const2_t = T;
  template <class T, class, class> using const3_t = T;
  template <class T, class...>     using constv_t = T;

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
