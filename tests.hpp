#include <exception>
#include <stdexcept>
#include <type_traits>

#include "curtains.hpp"
#include "ackermann.hpp"
#include "arith.hpp"
#include "arity.hpp"
#include "util.hpp"
#include "zero.hpp"
#include "fix_fun.hpp"
#include "rec_err.hpp"
#include "swap.hpp"

// g++ -std=c++1z tests.hpp && g++ -DCURTAINS_N -std=c++1z tests.hpp
// or
// FILE=tests.hpp && g++ -std=c++1z $FILE && g++ -DCURTAINS_N -std=c++1z $FILE

namespace static_asserts {

using namespace curtains;
using std::is_same_v;

static_assert(is_same_v<id,eval<>>);
static_assert(is_same_v<int,eval<int>>);

#ifndef CURTAINS_N
static_assert(is_same_v<int*,eval<quote<std::add_pointer_t>,int>>);
static_assert(is_same_v<int*,eval<quote_c<std::add_pointer>,int>>);
#else
using ap_qn = bases<quote<std::add_pointer_t>,ic<1>>;
static_assert(is_same_v<int*,eval<ap_qn,int>>);
#endif

// id
static_assert(is_same_v<id,eval<id,id>>);
static_assert(is_same_v<int,eval<id,const_,int,float>>);
static_assert(is_same_v<int,eval<id,id,const_,int,float>>);

// const_
static_assert(is_same_v<int,eval<const_,int,float>>);
static_assert(is_same_v<int,eval<const2,int,float>>);
static_assert(is_same_v<int,eval<eval<const2>,int,float>>);
static_assert(is_same_v<int,eval<const3,int,float,double>>);
static_assert(is_same_v<int,eval<eval<const3,int,float>,double>>);
static_assert(is_same_v<int,eval<eval<eval<const3,int>,float>,double>>);
static_assert(is_same_v<int,eval<eval<const_,int>,float>>);

#ifdef CURTAINS_N
static_assert(is_same_v<bool,eval<constv3,bool,char,int>>);
static_assert(is_same_v<bool,eval<eval<constv3,bool,char>,double>>);
#else
#ifndef CURTAINS_V_SIMPLE
static_assert(is_same_v<int,eval<constv,int,float,double,long,short>>);
#endif
#endif

// compose (.)
// (.) const id 42 42 == 42
static_assert(is_same_v<int,eval<compose,id,id,int>>);
static_assert(is_same_v<int,eval<eval<compose,id,id>,int>>);
static_assert(is_same_v<int,eval<compose,const_,id,int,char>>);
//  (.) const const 1 2 3 == 1
static_assert(is_same_v<int,eval<compose,const_,const_,int,char,bool>>);
static_assert(is_same_v<int,eval<eval<compose,const_,const_>,int,char,bool>>);

// cons
static_assert(is_same_v<list<double*>,eval<cons,double*,list<>>>);
static_assert(is_same_v<list<int,char>,eval<cons,int,list<char>>>);

// foldl
// foldl const 1 [2,3] == const (const (1) 2) 3 == 1
// foldl const 1 [] == 1
static_assert(is_same_v<char,eval<const_,char,double>>);
static_assert(is_same_v<char,eval<foldl,const_,char,list<>>>);
static_assert(is_same_v<char,eval<foldl,const_,char,list<int,float>>>);

#ifdef CURTAINS_V
static_assert(is_same_v<
                char,
                eval<foldl,quote<std::common_type_t>,char,list<>>
              >);
#ifndef CURTAINS_V_SIMPLE
static_assert(is_same_v<
                int,
                eval<foldl,quote<std::common_type_t>,char,list<char,int>>
              >);
#endif
static_assert(eval<
                foldl,
                quote<std::is_base_of>,
                std::exception,
                list<std::logic_error>
              >::value);
#endif

// foldr
static_assert(is_same_v<
                list<int,char,short>,
                eval<foldr,cons,list<>,list<int,char,short>>
              >);
using id_foldr_test = eval<foldr,cons,list<>>;
static_assert(is_same_v<
                list<int,char>,
                eval<id_foldr_test,list<int,char>>
              >);
static_assert(is_same_v<char,eval<foldr,id,char,list<id>>>);

// map
using map_  = eval<compose,eval<flip,foldr,list<>>,eval<compose,cons>>;
template <class F>
using map__ = eval<foldr,eval<compose,cons,F>,list<>>;

template <class map>
struct map_tests : ic<true> {
static_assert(is_same_v<list<>,eval<map,id,list<>>>);
static_assert(is_same_v<list<int>,eval<map,id,list<int>>>);
static_assert(is_same_v<list<int,char>,eval<map,id,list<int,char>>>);
static_assert(is_same_v<
                list<list<int>,list<char>>,
                eval<map,eval<flip,cons,list<>>,list<int,char>>
              >);
};
static_assert(map_tests<map>::value);
static_assert(map_tests<map_>::value);
#ifndef CURTAINS_N
static_assert(map_tests<quote<map__>>::value);
#else
static_assert(map_tests<bases<quote<map__>,ic<1>>>::value);
#endif

// eq
static_assert(is_same_v<std::true_type,eval<eq,int,int>>);
static_assert(is_same_v<std::false_type,eval<eq,int,char>>);

// flip
static_assert(is_same_v<int,eval<flip,const_,float,int>>);

// fix
static_assert(typename impl::fix_fact_c<int, ic<0>>::type{} == 1);
static_assert(typename impl::fix_fact_c<char,ic<0>>::type{} == 1);
static_assert(typename impl::fix_fact_c<id,ic<1>>::type{} == 0);  // 1 * 0
static_assert(typename impl::fix_fact_c<id,ic<7>>::type{} == 42); // 7 * 6

/*

  While Haskell is happy with a unary definition of fix:
  let rec fix f = f (fix f)
  ...it fails in the C++ template code above, as it does in OCaml.
  OCaml does though allow a binary formation:
  let rec fix f x = f (fix f) x
  ...and so it is with the C++ curtains library; see fix above. This is likely
  due to implicit currying: an application such as "fix f" being only a partial
  application of "fix f x".

# let rec fix f = f (fix f);;
val fix : ('a -> 'a) -> 'a = <fun>
# let fact f n = if n==0 then 1 else n * f (n-1);;
val fact : (int -> int) -> int -> int = <fun>
# fix fact 5;;
Stack overflow during evaluation (looping recursion?).
# let rec fix f x = f (fix f) x;;
val fix : (('a -> 'b) -> 'a -> 'b) -> 'a -> 'b = <fun>
# fix fact 5;;
- : int = 120
*/

static_assert(eval<fix,fix_fact,ic<0>>{} == 1);
static_assert(eval<fix,fix_fact,ic<1>>{} == 1);
static_assert(eval<fix,fix_fact,ic<2>>{} == 2);
static_assert(eval<fix,fix_fact,ic<3>>{} == 6);
//static_assert(eval<fix,fix_fact_if,ic<0>>{} == 1);
//static_assert(eval<fix,fix_fact_if,ic<3>>{} == 6);

static_assert(is_same_v<
                list<int,char,short>,
                eval<fix,fix_foldr,cons,list<>,list<int,char,short>>
              >);

// dollar ($)
static_assert(is_same_v<double,eval<dollar,id,double>>);
static_assert(is_same_v<double,eval<dollar,id,id,double>>);
static_assert(is_same_v<ic<1>,eval<eval<flip,dollar,ic<0>>,succ>>);
// static_assert( 2 == eval<dollar,succ,succ,ic<0>>::value); // see dollar.hpp

// if_
static_assert(is_same_v<int,  eval<if_,std::true_type, list<int>,list<float>>>);
static_assert(is_same_v<float,eval<if_,std::false_type,list<int>,list<float>>>);
static_assert(is_same_v<int,eval<
                              if_,
                              std::true_type,
                              list<int>,
                              list<rec_err,ic<3>>
                            >>);
static_assert(is_same_v<int,eval<
                              if_,
                              std::false_type,
                              list<rec_err,ic<3>>,
                              list<int>
                            >>);
// eval<rec_err,ic<3>> k;

// ski combinators (S, K and I)
// Wikipedia: SKSK = K and S(K(SI))K reverses the following two terms
static_assert(is_same_v<K,eval<S,K,S,K>>);
static_assert(is_same_v<bool,eval<S,eval<K,eval<S,I>>,K,bool,id>>);

// ack - ackermann function
static_assert(is_same_v<list<void>,eval<ack,list<>,list<>>>);

static_assert(is_same_v<
                list<void,void,void>,
                eval<ack,list<>,list<void,void>>
              >);

static_assert(is_same_v<
                list<void,void,void>,
                eval<ack,list<void,void>,list<>>
              >);

static_assert(is_same_v<
                list<void,void,void,void,void,void,void>,
                eval<ack,list<void,void>,list<void,void>>
              >);

// arith - add, subtract, factorial
static_assert(is_same_v<ic<4>,eval<add,ic<1>,ic<3>>>);
static_assert(is_same_v<ic<1>,eval<fact,ic<0>>>);
static_assert( 4 == eval<add,ic<1>,ic<3>>::value);
static_assert( 4 == eval<subtract,ic<7>,ic<3>>::value);
static_assert( 6 == eval<mul,ic<2>,ic<3>>::value);
static_assert( 2 == eval<divide,ic<6>,ic<3>>::value);
static_assert( 1 == eval<succ,ic<0>>::value);
static_assert(-1 == eval<pred,ic<0>>::value);
static_assert( 1 == eval<fact,ic<0>>{});
static_assert( 1 == eval<fact,ic<1>>{});
static_assert( 2 == eval<fact,ic<2>>{});
static_assert( 6 == eval<fact,ic<3>>{});
static_assert( 2 == eval<succ,eval<succ,ic<0>>>::value);

#ifdef CURTAINS_N
static_assert(is_same_v<ic<0>,eval<arity,void>>);
static_assert(is_same_v<ic<1>,eval<arity,id>>);
static_assert(is_same_v<ic<1>,eval<arity,eval<id,id>>>);
static_assert(is_same_v<ic<2>,eval<arity,eval<id,const_>>>);
static_assert(is_same_v<ic<2>,eval<arity,const_>>);
static_assert(is_same_v<ic<1>,eval<arity,eval<const_,constv3>>>);
static_assert(is_same_v<ic<3>,eval<arity,eval<const_,constv3,int>>>);
static_assert(is_same_v<ic<3>,eval<arity,constv3>>);

// Prelude> :t const
// const :: a -> b -> a
// Prelude> :t const . const
// const . const :: a -> b1 -> b -> a
// curtains thinks the arity is 1:
static_assert(is_same_v<ic<1>,eval<arity,eval<const_,const_>>>);
// ...but, it is! or can be seen so...
// as in... a -> b1 -> b -> a is a unary function returning a binary function.
// Consider compose: in compose.hpp we define its arity to 3, but it could
// just as well be understood as having an arity of 2.
// At least Haskell's type checker can give a consistent arity value:
//   the number of arrows which are not contained within parentheses.

static_assert(is_same_v<ic<1>,eval<arity,eval<compose,id,id>>>);
static_assert(is_same_v<ic<2>,eval<arity,eval<compose,id,id,const_>>>);
static_assert(is_same_v<ic<3>,eval<arity,eval<compose,id,id,const3>>>);
#endif // CURTAINS_N

// has_type_member
static_assert( has_type_member_v<tt<std::is_pointer,int*>>);
static_assert(!has_type_member_v<tt<std::is_pointer,int >>);
static_assert( has_type_member_v<tt<std::is_same,int,int >>);
static_assert(!has_type_member_v<tt<std::is_same,int,char>>);

// util
static_assert( std::is_same_v<void,is_pointer_tt<int*>>);
static_assert( std::is_same_v<void,is_same_tt<int,int>>);

static_assert(!has_type_member_v<int>);
static_assert( has_type_member_v<impl::id_c<int>>);
static_assert( has_type_member_v<impl::invoke_m<void,impl::id_c,int>>);
static_assert( has_type_member_v<impl::xt<int>>);
static_assert(!has_type_member_v<impl::xt<impl::xt<int>>>);

static_assert(!can_invoke_v<int,int>); // tough nut
static_assert( can_invoke_v<id,int>);
static_assert(!can_invoke_v<id,int,double>);
static_assert( can_invoke_v<id,int>);
static_assert(!can_invoke_v<id,int,int>);
static_assert( can_invoke_v<impl::curry<id,int>>);
static_assert( can_invoke_v<impl::curry<id>,int>);
// Is invoke<invalid<F>,T> == invoke<invalid,curry<F,T>> ? Yes. Next 2 lines:
static_assert(!can_invoke_v<impl::invalid<id>,int>);
static_assert(!can_invoke_v<impl::invalid<impl::curry<id,int>>>);
static_assert(!can_invoke_v<impl::curry<impl::invalid<id>,int>>);

// get
static_assert(is_same_v<int,eval<get,list<int>,ic<0>>>);
static_assert(is_same_v<int,eval<get,list<int,char>,ic<0>>>);
static_assert(is_same_v<char,eval<get,list<int,char>,ic<1>>>);
static_assert(is_same_v<int,eval<get,list<int,char,float>,ic<0>>>);
static_assert(is_same_v<char,eval<get,list<int,char,float>,ic<1>>>);
static_assert(is_same_v<float,eval<get,list<int,char,float>,ic<2>>>);
static_assert(is_same_v<float,eval<get,bases<int,char,float>,ic<2>>>);

// get_s
static_assert(is_same_v<eval<get,list<int>>,get_s<list<int>>>);
static_assert(is_same_v<
                eval<get,list<int>,ic<0>>,
                get_s<list<int>,ic<0>>
              >);

// ic
static_assert(is_same_v<int,ic< 0>::value_type>);
static_assert(is_same_v<int,ic<-1>::value_type>);

template <class>
  struct grip;
template <auto N>
struct grip<ic<N>> { using type = decltype(N); };
static_assert(is_same_v<int,grip<ic< 0>>::type>);
static_assert(is_same_v<int,grip<ic<-1>>::type>);

// curry
#ifndef CURTAINS_N
static_assert(is_same_v<eval<const_,int>,eval<impl::curry<const_,int>>>);
#endif

// tree
static_assert(is_same_v<tree<int,list<>>,eval<fmap_tree,id,tree<int,list<>>>>);
static_assert(is_same_v<
                tree<char,list<>>,
                eval<fmap_tree,eval<const_,char>,tree<int,list<>>>
              >);
static_assert(is_same_v<
                tree<char,list<tree<char,list<>>,tree<
                                                   char,
                                                   list<tree<char,list<>>>
                                                 >>>,
                eval<
                  fmap_tree,
                  eval<const_,char>,
                  tree<char,list<tree<long,list<>>,tree<
                                                     int,
                                                     list<tree<void,list<>>>
                                                   >>>
                >
              >);

// swap
//static_assert(is_same_v<eval<swap,quote<lazy>,quote<list>,lazy<int>>,list<int>>);
using type1 = lazy<mul,ic<2>,lazy<id,lazy<pred,ic<4>>>>;
using type2 = list<mul,ic<2>,list<id,list<pred,ic<4>>>>;
//static_assert(is_same_v<type2,eval<swap,quote<lazy>,quote<list>,type1>>);
//static_assert(is_same_v<ic<6>,eval<swap,quote<lazy>,quote<eval>,type1>>);

#if 0
// zero - zero arity function tests
static_assert(is_same_v<id,eval<zero_id>>);
static_assert(is_same_v<float,eval<zero_id,float>>);

//static_assert(is_same_v<bool,eval<invoke_if,zero_const,bool,char>>);
//static_assert(is_same_v<bool,eval<invoke_if,zero_constB,bool,char>>);
static_assert(is_same_v<eval<zero_const>,const_>);
static_assert(is_same_v<eval<zero_const,bool,char>,bool>);
static_assert(is_same_v<eval<zero_constB,bool,char>,bool>);
static_assert(is_same_v<eval<id,zero_const,bool,char>,bool>);
static_assert(is_same_v<eval<id,zero_constB,bool,char>,bool>);

static_assert(is_same_v<char,eval<const_,zero_const,int,char,bool>>);

static_assert(is_same_v<eval<zero_test>,int>);
static_assert(is_same_v<eval<id,zero_test>,int>); // recursive invoke_if
static_assert(is_same_v<eval<zero_testB>,int>);   // ""

// variadic zero arity
#ifdef CURTAINS_V
static_assert(is_same_v<const_,eval<zero_constv>>);
static_assert(is_same_v<const_,eval<zero_constv,int>>);
static_assert(is_same_v<const_,eval<zero_constv,int,float>>);
#else
static_assert(is_same_v<const_,eval<zero_constv0>>);
static_assert(is_same_v<int,eval<eval<zero_constv0,int>,float>>);
static_assert(is_same_v<int,eval<zero_constv0,int,float>>);
#endif

#endif

} // namespace static_asserts
