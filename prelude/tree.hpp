#ifndef __TREE_HPP__
#define __TREE_HPP__

// A multi-way, or rose tree.

#include "../curtains/curtains_includes.hpp"
#include "list.hpp"

namespace curtains {

  template <class, class>
    struct tree;

  template <class A, class ...As>
  struct tree<A,list<As...>> {};

} // namespace curtains

namespace curtains::impl {

  template <class, class>
    struct fmap_tree_c;

  template <class F, class T>
  using fmap_tree_t = typename fmap_tree_c<F,T>::type;

} // namespace curtains::impl

namespace curtains::v {

  using fmap_tree = quote<impl::fmap_tree_t>;

} // namespace curtains::v

namespace curtains::n {

  using fmap_tree = bases<v::fmap_tree,ic<2>>;

} // namespace curtains::n

namespace curtains::impl {

  template <class F, class A, class L>
  struct fmap_tree_c<F,tree<A,L>>
    : id_c<tree<eval<F,A>,eval<map,eval<fmap_tree,F>,L>>> {};

} // namespace curtains::impl

#endif // __TREE_HPP__
