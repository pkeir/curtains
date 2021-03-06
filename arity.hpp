#ifndef __ARITY_HPP__
#define __ARITY_HPP__

#include "curtains.hpp"

namespace curtains::impl::n {

  // This could equally be a SFINAE class which looks 4 an m_arity type member.
  template <class T>
  struct arity_c             : id_c<ic<0>> {};

  template <class T, class N>
  struct arity_c<bases<T,N>> : id_c<N>     {};

} // namespace curtains::impl:n

namespace curtains::v {

  // nothing

} // namespace curtains::v

namespace curtains::n {

  using arity = bases<quote_c<impl::n::arity_c>,ic<1>>;

} // namespace curtains::n

#endif // __ARITY_HPP__
