#ifndef __ZERO_HPP__
#define __ZERO_HPP__

#include "curtains.hpp"

namespace curtains::impl {

  template <class...> struct zero_id_c;

  template <class ...Ts>
  using zero_id_t = typename zero_id_c<Ts...>::type;

  template <class...> struct zero_constv_c;

  template <class ...Ts>
  using zero_constv_t = typename zero_constv_c<Ts...>::type;

  template <class...> struct zero_const_c;

  template <class ...Ts>
  using zero_const_t = typename zero_const_c<Ts...>::type;

  template <class...> struct zero_constB_c;

  template <class ...Ts>
  using zero_constB_t = typename zero_constB_c<Ts...>::type;

  template <class...> struct zero_test_c;

  template <class ...Ts>
  using zero_test_t = typename zero_test_c<Ts...>::type;

  template <class...> struct zero_testB_c;

  template <class ...Ts>
  using zero_testB_t = typename zero_testB_c<Ts...>::type;

} // namespace curtains::impl

namespace curtains::v {

  using zero_id      = quote<impl::zero_id_t>;
  using zero_constv  = quote<impl::zero_constv_t>;
  using zero_const   = quote<impl::zero_const_t>;
  using zero_constB  = quote<impl::zero_constB_t>;
  using zero_test    = quote<impl::zero_test_t>;
  using zero_testB   = quote<impl::zero_testB_t>;

} // namespace curtains::v

namespace curtains::n {

  using zero_id      = bases<v::zero_id,ic<0>>;
  using zero_constv0 = bases<v::zero_constv,ic<0>>; // n.b. != v::zero_constv
  using zero_const   = bases<v::zero_const,ic<0>>;
  using zero_constB  = bases<v::zero_constB,ic<0>>;
  using zero_test    = bases<v::zero_test,ic<0>>;
  using zero_testB   = bases<v::zero_testB,ic<0>>;

} // namespace curtains::n

namespace curtains::impl {

  template <        > struct zero_id_c<>     : id_c<id>         {};
  template <class...> struct zero_constv_c   : id_c<const_>     {};
  template <        > struct zero_const_c<>  : id_c<const_>     {};
  template <        > struct zero_constB_c<> : id_c<zero_const> {};
  template <        > struct zero_test_c<>   : id_c<int>        {};
  template <        > struct zero_testB_c<>  : id_c<zero_test>  {};

} // namespace curtains::impl

#endif // __ZERO_HPP__
