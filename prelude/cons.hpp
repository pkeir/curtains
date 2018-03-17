#ifndef __CONS_HPP__
#define __CONS_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class, class>
    struct cons_c;

  template <class T, template <class ...> class TT, class ...Ts>
  struct cons_c<T,TT<Ts...>> : id_c<TT<T,Ts...>> {};

} // namespace curtains

namespace curtains::v {

  using cons = quote_c<impl::cons_c>;

} // namespace curtains::v

namespace curtains::n {

  using cons = bases<v::cons,ic<2>>;

} // namespace curtains::n

#endif // __CONS_HPP__
