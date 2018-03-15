#ifndef __SKI_HPP__
#define __SKI_HPP__

#include "curtains.hpp"

namespace curtains::impl {

  template <class X, class Y, class Z>
  using S_t = eval<X,Z,eval<Y,Z>>;

} // namespace curtains::impl

namespace curtains::v {

  using I = id;
  using K = const_;
  using S = quote<impl::S_t>;

} // namespace curtains::v

namespace curtains::n {

  using I = id;
  using K = const_;
  using S = bases<v::S,ic<3>>;

} // namespace curtains::n

#endif // __SKI_HPP__
