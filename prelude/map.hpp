#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "../curtains/curtains_includes.hpp"
#include "list.hpp"

// map :: (a ->  b) ->  ([a] ->  [b])
// map f = foldr (\ x xs -> f x : xs) []
// map = \f -> foldr (\ x xs -> f x : xs) []
// pointfree:
// flip fold ([]) . ((:) .)
// (.) (flip foldr []) ((.) (:))

namespace curtains::impl {

  template <class, class>
    struct map_c;

} // namespace curtains::impl

namespace curtains::v {
  
  using map  = quote_c<impl::map_c>;

} // namespace curtains::v

namespace curtains::n {

  using map = bases<v::map,ic<2>>;

} // namespace curtains::n

namespace curtains::impl {

  template <class F, class ...Ts>
  struct map_c<F,list<Ts...>> : id_c<list<eval<F,Ts>...>> {};

} // namespace curtains::impl

#endif // __MAP_HPP__
