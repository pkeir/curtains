#ifndef __EVAL_V_HPP__
#define __EVAL_V_HPP__

#include "core.hpp"

namespace curtains::impl {

  template <class, class = void_t<>>
  struct xt : id_c<void> {};

  template <class T>
  struct xt<T,void_t<typename T::type>> {};

  template <class F>
  struct invalid {
    template <class ...Us>
    using m_invoke = typename xt<
                       impl::invoke_m<void,F::template m_invoke,Us...>
                     >::type;
  };

} // namespace curtains::impl

// The simplest "v" implementation does not handle variadic metafunctions:
// #define CURTAINS_V_SIMPLE

namespace curtains::impl::v {

  template <class F, class = void_t<>>
  struct invoke_if_c                      :        id_c<F>         {};

#ifdef CURTAINS_V_SIMPLE
  template <class F>
  struct invoke_if_c<F,void_t<invoke<F>>> :        id_c<invoke<F>> {};
#else
  template <class F>
  struct invoke_if_c<F,void_t<invoke<F>>> : invoke_if_c<invoke<F>> {};
#endif

  template <class F>
  using invoke_if_t  = typename invoke_if_c<F>::type;
  using invoke_if    = quote<invoke_if_t>;

#ifdef CURTAINS_V_SIMPLE
  template <class F, class T>
  using curry_join_t = curry<invoke_if_t<F>,T>;
#else
  template <class F, class T, class = void_t<>>
  struct curry_join_c                     : id_c<curry<F,T>> {};

  template <class F, class T>
  struct curry_join_c<F,T,void_t<invoke<F>,invoke<invalid<F>,T>>>
       : curry_join_c<invoke<F>,T> {};

  template <class F, class T>
  using curry_join_t = typename curry_join_c<F,T>::type;
#endif

  using curry_join = quote<curry_join_t>;

} // namespace curtains::impl::v

namespace curtains::v {

  using id   = quote<impl::id_t>;    // Rather place this in id.hpp

  template <class ...Fs>
  using eval = impl::v::invoke_if_t<
                 impl::invoke<impl::v::ifoldl,impl::v::curry_join,id,Fs...>
               >;

} // namespace curtains::v

#endif // __EVAL_V_HPP__
