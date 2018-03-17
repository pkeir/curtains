#ifndef __FIX_HPP__
#define __FIX_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class,class>
    struct fix_c;

  template <class F, class X>
  using fix_t = typename fix_c<F,X>::type;

} // namespace curtains::impl

namespace curtains::v {

  using fix = quote_c<impl::fix_c>;

} // namespace curtains::v

namespace curtains::n {

  using fix = bases<v::fix,ic<2>>;

} // namespace curtains::n

namespace curtains::impl {

  template <class F, class X>
  struct fix_c : id_c<eval<F,eval<fix,F>,X>> {};

  // No. Template explosion.
/*  template <class F, auto N>
  struct fix_fact2_c<F,ic<N>> : id_c<
                                  eval<
                                    if_,
                                    std::is_same<ic<N>,ic<0>>,
                                    ic<1>,
                                    ic<N*eval<F,ic<N-1>>::type::value>
                                  >
                                > {};
*/

} // namespace curtains::impl

#endif // __FIX_HPP__
