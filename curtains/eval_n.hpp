#ifndef __EVAL_N_HPP__
#define __EVAL_N_HPP__

#include "core.hpp"

namespace curtains {

  template <auto I> using ic = std::integral_constant<decltype(I),I>;

  template <class ...Ts> struct bases   : Ts... {};
  template <>            struct bases<>         {};

} // namespace curtains

namespace curtains::impl::n {

  using curtains::impl::curry;
  using curtains::impl::invoke;

  template <class, class>
    struct curry_join_c;

  template <class F, class T, auto N>
  struct curry_join_c<bases<F,ic<N>>,T> : id_c<bases<curry<F,T>,ic<N-1>>> {};

  template <class F, class T>
  struct curry_join_c<bases<F,ic<1>>,T> : id_c<invoke<F,T>> {};

  template <class F, class T>
  using curry_join_t = typename curry_join_c<F,T>::type;

  template <class, class>
    struct curry_join0_c;

  template <class F, class T, auto N>
  struct curry_join0_c<bases<F,ic<N>>,T> : id_c<bases<curry<F,T>,ic<N-1>>> {};

  template <class F, class T>
  struct curry_join0_c<bases<F,ic<0>>,T> :
         curry_join0_c<invoke<bases<F,ic<0>>>,T> {};

  template <class F, class T>
  using curry_join0_t = typename curry_join0_c<F,T>::type;

  template <class F>
  struct invoke_if_c                 : id_c<F>       {};

  template <class F>
  struct invoke_if_c<bases<F,ic<0>>> : invoke_if_c<invoke<bases<F,ic<0>>>> {};

  template <class F>
  using invoke_if_t  = typename invoke_if_c<F>::type;

  using invoke_if    = quote<invoke_if_t>;

  using curry_join = quote<curry_join_t>;
  using curry_join0 = quote<curry_join0_t>;

} // namespace curtains::impl::n

namespace curtains::n {

  using id = bases<quote<impl::id_t>,ic<1>>; // move this into id.hpp

  // To remove support for zero-arity functions, use impl::curry_join
  // rather than curry_join, and remove the use of impl::n::invoke_if_t
  template <class ...Fs>
  using eval = impl::n::invoke_if_t<
                 impl::invoke<impl::v::ifoldl,impl::n::curry_join0,id,Fs...>
               >;

} // namespace curtains::n

#endif // __EVAL_N_HPP__
