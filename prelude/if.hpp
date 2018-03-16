#ifndef __IF_HPP__
#define __IF_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  template <class, class, class>
    struct if_c;

  template <class V, template <class ...> class TT, class ...Ts>
  struct if_c<std::true_type, TT<Ts...>,V>  : id_c<eval<Ts...>> {};

  template <class U, template <class ...> class TT, class ...Ts>
  struct if_c<std::false_type,U,TT<Ts...>>  : id_c<eval<Ts...>> {};

  template <class T, class U, class V>
  using if_t = typename if_c<T,U,V>::type;

} // namespace curtains::impl

namespace curtains::v {

  using if_ = quote<impl::if_t>;

} // namespace curtains::v

namespace curtains::n {

  using if_ = bases<v::if_,ic<3>>;

} // namespace curtains::n

#endif // __IF_HPP__
