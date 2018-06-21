#ifndef __ARITH_HPP__
#define __ARITH_HPP__

#include "curtains.hpp"

namespace curtains::impl {

  template <class>        struct succ_c;
  template <class>        struct pred_c;
  template <class, class> struct add_c;
  template <class, class> struct subtract_c;
  template <class, class> struct mul_c;
  template <class, class> struct divide_c;
  template <class>        struct fact_c;

} // namespace curtains::impl

namespace curtains::v {

  using succ     = quote_c<impl::succ_c>;
  using pred     = quote_c<impl::pred_c>;
  using add      = quote_c<impl::add_c>;
  using subtract = quote_c<impl::subtract_c>;
  using mul      = quote_c<impl::mul_c>;
  using divide   = quote_c<impl::divide_c>;
  using eq       = quote_c<std::is_same>;    // move somewhere more general
  using fact     = quote_c<impl::fact_c>;

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
