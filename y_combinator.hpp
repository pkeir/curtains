#ifndef __Y_COMBINATOR_HPP__
#define __Y_COMBINATOR_HPP__

#include "curtains.hpp"

namespace curtains::impl {

  template <class F, class X, class A>
  using y_helper_t = eval<F,eval<X,X>,A>;

} // namespace curtains::impl

namespace curtains::v {

  using y_helper = quote<impl::y_helper_t>;

} // namespace curtains::v

namespace curtains::n {

  using y_helper = bases<v::y_helper,ic<3>>;

} // namespace curtains::n

namespace curtains::impl {

  template <class F, class G>
  using y_t = eval<eval<y_helper,F>,eval<y_helper,F>,G>;

} // namespace curtains::impl

namespace curtains::v {

  using y = quote<impl::y_t>;

} // namespace curtains::v

namespace curtains::n {

  using y = bases<v::y,ic<2>>;

} // namespace curtains::n

#endif // __Y_COMBINATOR_HPP__
