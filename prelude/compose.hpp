#ifndef __COMPOSE_HPP__
#define __COMPOSE_HPP__

#include "../curtains/curtains_includes.hpp"

// #define EXPLICIT_NESTING

#ifndef EXPLICIT_NESTING

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

#else // EXPLICIT_NESTING

namespace curtains::impl {

  // Ye olde way
  template <class F, class G>
  class compose
  {
    template <class T>
    using m_invoke = eval<F,eval<G,T>>;
    public:
    using type = bases<quote<m_invoke>,ic<1>>; // Alas, "bases" is pollution
  };

} // namespace curtains::impl

namespace curtains::v {

  using compose = quote_c<impl::compose>;

} // namespace curtains::v

namespace curtains::n {

  using compose = bases<v::compose,ic<2>>;

} // namespace curtains::n

#endif // EXPLICIT_NESTING

#endif // __COMPOSE_HPP__
