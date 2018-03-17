#ifndef __FOLDL_HPP__
#define __FOLDL_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class, class, class>
    struct foldl_c;

  template <class F, class Z, class T>
  using foldl_t = typename foldl_c<F,Z,T>::type;

} // namespace curtains::impl

namespace curtains::v {

  using foldl = quote_c<impl::foldl_c>;

} // namespace curtains::v

namespace curtains::n {

  using foldl = bases<v::foldl,ic<3>>;

} // namespace curtains::n

namespace curtains::impl {

  template <
    class F,
    class Z,
    class T,
    template <class ...> class TT,
    class ...Ts
  >
  struct foldl_c<F,Z,TT<T,Ts...>> :id_c<eval<foldl,F,eval<F,Z,T>,TT<Ts...>>>{};

  template <class F, class Z, template <class...> class TT>
  struct foldl_c<F,Z,TT<>>        :id_c<Z>                                  {};

} // namespace curtains::impl

#endif // __FOLDL_HPP__
