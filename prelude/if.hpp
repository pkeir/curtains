#ifndef __IF_HPP__
#define __IF_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class, class, class>
    struct if_c;

  template <class, class, class>
    struct lazy_if_c;

  template <class V, template <class ...> class TT, class ...Ts>
  struct lazy_if_c<std::true_type, TT<Ts...>,V>  : id_c<eval<Ts...>> {};

  template <class U, template <class ...> class TT, class ...Ts>
  struct lazy_if_c<std::false_type,U,TT<Ts...>>  : id_c<eval<Ts...>> {};

} // namespace curtains::impl

namespace curtains::v {

  using      if_ = quote_c<impl::if_c>;
  using lazy_if_ = quote_c<impl::lazy_if_c>;

} // namespace curtains::v

namespace curtains::n {

  using      if_ = bases<v::if_,ic<3>>;
  using lazy_if_ = bases<v::lazy_if_,ic<3>>;

} // namespace curtains::n

namespace curtains::impl {

  template <class T, class U> struct if_c<std::true_type, T,U> : id_c<T> {};
  template <class T, class U> struct if_c<std::false_type,T,U> : id_c<U> {};

} // namespace curtains::impl

#endif // __IF_HPP__

