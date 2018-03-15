#ifndef __FIX_FUN_HPP__
#define __FIX_FUN_HPP__

#include "curtains.hpp"

namespace curtains::impl {

  template <class,class>
    struct fix_fact_c;

  template <class F, class N>
  using fix_fact_t = typename fix_fact_c<F,N>::type;

  template <class, class, class, class>
    struct fix_foldr_c;

  template <class Y, class F, class Z, class T>
  using fix_foldr_t = typename fix_foldr_c<Y,F,Z,T>::type;

  template <class,class>
    struct fix_fact_if_c;

  template <class F, class N>
  using fix_fact_if_t = typename fix_fact_if_c<F,N>::type;

} // namespace curtains::impl

namespace curtains::v {

  using fix_fact    = quote<impl::fix_fact_t>;
  using fix_foldr   = quote<impl::fix_foldr_t>;
  using fix_fact_if = quote<impl::fix_fact_if_t>;

} // namespace curtains::v

namespace curtains::n {

  using fix_fact    = bases<v::fix_fact,ic<2>>;
  using fix_foldr   = bases<v::fix_foldr,ic<4>>;
  using fix_fact_if = bases<v::fix_fact_if,ic<2>>;

} // namespace curtains::n

namespace curtains::impl {

//  template <class F, auto N>
//  struct fix_fact_c<F,ic<N>> :
//    id_c< ic<N*eval<F,ic<N-1>>::type::value> > {};
//    id_c< eval<mul,ic<N>,eval<F,ic<N-1>>> > {};

  template <class F, class N>
  struct fix_fact_c          : id_c<eval<mul,N,eval<F,eval<pred,N>>>> {};

  template <class F>
  struct fix_fact_c<F,ic<0>> : id_c<ic<1>> {};

  template <
    class Y,
    class F,
    class Z,
    class T,
    template <class ...> class TT,
    class ...Ts
  >
  struct fix_foldr_c<Y,F,Z,TT<T,Ts...>> :
    id_c<eval<F,T,eval<Y,F,Z,TT<Ts...>>>>{};

  template <class Y, class F, class Z, template <class...> class TT>
  struct fix_foldr_c<Y,F,Z,TT<>> :id_c<Z> {};

  template <class F, class N>
  struct fix_fact_if_c : id_c<
                           eval<
                             if_,
                             typename std::is_same<N,ic<0>>::type,
                             list<ic<1>>,
                             list<mul,N,eval<F,eval<pred,N>>> 
                           >
                         > {};

} // namespace curtains::impl

#endif // __FIX_FUN_HPP__
