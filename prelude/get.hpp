#ifndef __GET_HPP__
#define __GET_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class, class>
    struct get_c;

  template <class T, class U>
  using get_t = typename get_c<T,U>::type;

} // namespace curtains::impl

namespace curtains::v {

  using get = quote_c<impl::get_c>;

} // namespace curtains::v

namespace curtains::n {

  using get = bases<v::get,ic<2>>;

} // namespace curtains::n

namespace curtains::impl {

  template <template <class ...> class TT, class T, class ...Ts>
  struct get_c<TT<T,Ts...>,ic<0>> : id_c<T>                {};

  template <template <class ...> class TT, class T, auto N, class ...Ts>
  struct get_c<TT<T,Ts...>,ic<N>> : get_c<TT<Ts...>,ic<N-1>> {};

} // namespace curtains::impl

// This is something to examine further
template <class ...Ts>
using get_s = eval<get,Ts...>;

#endif // __GET_HPP__
