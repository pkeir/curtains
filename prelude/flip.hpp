#ifndef __FLIP_HPP__
#define __FLIP_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class F, class T, class U>
  using flip_t = eval<F,U,T>;

} //  namespace curtains::impl

namespace curtains::v {

  using flip = quote<impl::flip_t>;

} // namespace curtains::v

namespace curtains::n {

  using flip = bases<v::flip,ic<3>>;

} // namespace curtains::n

#endif // __FLIP_HPP__
