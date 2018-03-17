#ifndef __FOLDR_HPP__
#define __FOLDR_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class, class, class>
    struct foldr_c;

  template <class F, class Z, class T>
  using foldr_t = typename foldr_c<F,Z,T>::type;

} // namespace curtains::impl

namespace curtains::v {

  using foldr = quote_c<impl::foldr_c>;

} // namespace curtains::v

namespace curtains::n {

  using foldr = bases<v::foldr,ic<3>>;

} // namespace curtains::n

namespace curtains::impl {

  template <
    class F,
    class Z,
    class T,
    template <class ...> class TT,
    class ...Ts
  >
  struct foldr_c<F,Z,TT<T,Ts...>> :id_c<eval<F,T,eval<foldr,F,Z,TT<Ts...>>>>{};

  template <class F, class Z, template <class...> class TT>
  struct foldr_c<F,Z,TT<>>        :id_c<Z>                                  {};

} // namespace curtains::impl

#endif // __FOLDR_HPP__
