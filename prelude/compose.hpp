#ifndef __COMPOSE_HPP__
#define __COMPOSE_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::impl {

  /* Ye olde way
  template <class F, class G>
  struct compose_t
  {
    template <class T>
    using m_invoke = eval<F,eval<G,T>>;
  };
  */

  template <class F, class G, class T>
  using compose_t = eval<F,eval<G,T>>;

} // namespace curtains::impl

namespace curtains::v {

  using compose = quote<impl::compose_t>;

} // namespace curtains::v

namespace curtains::n {

  using compose = bases<v::compose,ic<3>>;

} // namespace curtains::n

#endif // __COMPOSE_HPP__
