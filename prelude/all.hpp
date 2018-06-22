#ifndef __ALL_HPP__
#define __ALL_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class P, class T>
  using all_t = eval<foldr,eval<compose,and_,P>,ic<true>,T>;

} // namespace curtains::impl

namespace curtains::v {

  using all = quote<impl::all_t>;

} // namespace curtains::v

namespace curtains::n {

  using all = bases<v::all,ic<2>>;

} // namespace curtains::n

#endif // __ALL_HPP__
