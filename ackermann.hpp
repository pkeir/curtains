#ifndef __ACKERMANN_HPP__
#define __ACKERMANN_HPP__

#include "curtains.hpp"
#include "ski.hpp"

/*
ack = foldr (\x g -> foldr (\y -> g) (g [1])) (1:)

\x g -> foldr (\y -> g) (g [1])
const (liftM2 foldr const ($ [1]))
const (pure foldr <*> const <*> ($ [1]))

ack = foldr (const (pure foldr <*> const <*> ($ [1]))) (1:)

($ [1])
flip ($) [1]

ack = foldr (const (pure foldr <*> const <*> (flip ($) [1]))) (1:)

// Now remove operators

ack = foldr (const ((<*>) ((<*>) (pure foldr) const) (flip ($) [1]))) (1:)

ack = foldr (const ((<*>) ((<*>) (pure foldr) const) (flip ($) [1]))) ((:) 1)

// We are looking at the Applicative instance of ((->) r)
// (<*>) is the S combinator (SKI), and pure is const.
ack = foldr (const ((<*>) ((<*>) (const foldr) const) (flip ($) [1]))) ((:) 1)

We could use ic<1> for the 1, but we might as well use void.

> (const ((<*>) ((<*>) (const foldr) const) (const []))) [] id []
[]
*/

namespace curtains::v {

  using ack = eval<
                foldr,
                eval<
                  const_,
                  eval<
                    S,
                    eval<
                      S,
                      eval<const_,foldr>,
                      const_
                    >,
                    eval<flip,dollar,list<void>>
                  >
                >,
                eval<cons,void>
              >;

} // namespace curtains::v

namespace curtains::n {

  using ack = eval<
                foldr,
                eval<
                  const_,
                  eval<
                    S,
                    eval<
                      S,
                      eval<const_,foldr>,
                      const_
                    >,
                    eval<flip,dollar,list<void>>
                  >
                >,
                eval<cons,void>
              >;

} // namespace curtains::n

#endif // __ACKERMANN_HPP__
