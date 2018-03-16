#ifndef __ARITH_HPP__
#define __ARITH_HPP__

#include "curtains.hpp"

namespace curtains::impl {

  template <class>        struct pred_c;
  template <class>        struct succ_c;
  template <class, class> struct add_c;
  template <class, class> struct subtract_c;
  template <class, class> struct mul_c;
  template <class, class> struct divide_c;
  template <class>        struct fact_c;

  template <class N>
  using pred_t     = typename pred_c<N>::type;

  template <class N>
  using succ_t     = typename succ_c<N>::type;

  template <class T, class U>
  using add_t      = typename add_c<T,U>::type;

  template <class T, class U>
  using subtract_t = typename subtract_c<T,U>::type;

  template <class T, class U>
  using mul_t      = typename mul_c<T,U>::type;

  template <class T, class U>
  using divide_t   = typename divide_c<T,U>::type;

  template <class T, class U>
  using eq_t       = typename std::is_same<T,U>::type;

  template <class N>
  using fact_t     = typename fact_c<N>::type;

} // namespace curtains::impl

namespace curtains::v {

  using pred     = quote<impl::pred_t>;
  using succ     = quote<impl::succ_t>;
  using add      = quote<impl::add_t>;
  using subtract = quote<impl::subtract_t>;
  using mul      = quote<impl::mul_t>;
  using divide   = quote<impl::divide_t>;
  using eq       = quote<impl::eq_t>;
  using fact     = quote<impl::fact_t>;

} // namespace curtains::v

namespace curtains::n {

  using succ     = bases<v::succ,ic<1>>;
  using pred     = bases<v::pred,ic<1>>;
  using add      = bases<v::add,ic<2>>;
  using subtract = bases<v::subtract,ic<2>>;
  using mul      = bases<v::mul,ic<2>>;
  using divide   = bases<v::divide,ic<2>>;
  using eq       = bases<v::eq,ic<2>>;
  using fact     = bases<v::fact,ic<1>>;

} // namespace curtains::v

namespace curtains::impl {

  template <auto N>         struct succ_c<ic<N>>            : id_c<ic<N+1>> {};
  template <auto N>         struct pred_c<ic<N>>            : id_c<ic<N-1>> {};
  template <auto M, auto N> struct subtract_c<ic<M>, ic<N>> : id_c<ic<M-N>> {};
  template <auto M, auto N> struct      mul_c<ic<M>, ic<N>> : id_c<ic<M*N>> {};
  template <auto M, auto N> struct   divide_c<ic<M>, ic<N>> : id_c<ic<M/N>> {};
  template <auto M, auto N> struct      add_c<ic<M>, ic<N>> : id_c<ic<M+N>> {};

  template <auto N>
  struct fact_c<ic<N>> : id_c<ic<N*eval<fact,ic<N-1>>::type::value>> {};

  template <>
  struct fact_c<ic<0>> : id_c<ic<1>> {};

} // namespace curtains::impl

#endif // __ARITH_HPP__
