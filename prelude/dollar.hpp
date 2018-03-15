#ifndef __DOLLAR_HPP__
#define __DOLLAR_HPP__

#include "../curtains/curtains_includes.hpp"

/*
  This is not a proper implementation of ($). The problem is that ($)
  should have lowest priority. The implementation below will work in
  some situations, but will fail with something like "(+1) $ (+1) 0".
  An equivalent problem in Curtains could be: eval<succ,succ,ic<0>>.
  Curtains tests will pass if:     f $ x y z == (f x) y z
  ...but "flip $ x" also works, which is used in the Ackermann function.
  The priorities for Haskell are listed below.

  infixr 9  .
  infixr 8  ^, ^^, **
  infixl 7  *, /, `quot`, `rem`, `div`, `mod`
  infixl 6  +, -
  infixr 5  :
  infix  4  ==, /=, <, <=, >=, >
  infixr 3  &&
  infixr 2  ||
  infixl 1  >>, >>=
  infixr 1  =<<
  infixr 0  $, $!, `seq`
*/

namespace curtains::impl {

  // Equivalent to the Haskell ($): ($) :: (a -> b) -> a -> b
  //                                f $ x = f x

  template <class F, class T>
  using dollar_t = eval<F,T>;

} //  namespace curtains::impl

namespace curtains::v {

  using dollar = quote<impl::dollar_t>;

} // namespace curtains::v

namespace curtains::n {

  using dollar = bases<v::dollar,ic<2>>;

} // namespace curtains::n

#endif // __DOLLAR_HPP__
