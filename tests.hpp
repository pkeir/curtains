#include <exception>
#include <stdexcept>
#include <type_traits>

#include "curtains.hpp"
#include "ackermann.hpp"
#include "arity.hpp"
#include "util.hpp"
#include "zero.hpp"
#include "fix_fun.hpp"
#include "rec_err.hpp"
#include "swap.hpp"
#include "y_combinator.hpp"

// g++ -std=c++1z tests.hpp && g++ -DCURTAINS_N -std=c++1z tests.hpp
// or
// FILE=tests.hpp && g++ -std=c++1z $FILE && g++ -DCURTAINS_N -std=c++1z $FILE

namespace static_asserts {

using namespace curtains;
using std::is_same_v;

static_assert(is_same_v<id,eval<>>);
static_assert(is_same_v<int,eval<int>>);
static_assert(is_same_v<std::true_type,ic<true>>);
static_assert(is_same_v<std::false_type,ic<false>>);
static_assert(is_same_v<std::integral_constant<int,42>,ic<42>>);

#ifdef CURTAINS_V
template <class T, class U>
struct const_test {
  using type = T;
};
using const_test_q = quote_c<const_test>;
static_assert(is_same_v<int,eval<const_test_q,int,float>>);

struct const_test2 {
  template <class T, class U>
  using m_invokey = T;
  template <class ...Ts>
  using m_invoke = typename impl::invoke_m<void,m_invokey,Ts...>::type;
};
static_assert(is_same_v<int,eval<const_test2,int,float>>);

// This fails as const_test3_q cannot be applied to *both* int and float. But
// const_test3_q *can* be applied to int; this "returns" const_test3<int>,
// a type with an m_invoke member. This is though not quite in the correct
// form: it isn't variadic. The m_invoke member created from quote or quote_c
// is variadic.
template <class T>
struct const_test3 {
  template <class U>
  using m_invoke = T;
};
//static_assert(is_same_v<int,eval<const_test3<int>,float>>); // fails
using const_test3_q = quote<const_test3>;
//static_assert(is_same_v<int,eval<const_test3_q,int,float>>); // also fails

template <class T>
struct const_test4 {
  template <class U>
  using m_invoke = T;
  using type = quote<m_invoke>;
};

// We will need quote_c now to get the type member:
// Todo: create a new quote variant to help with this
using const_test4_q = quote_c<const_test4>;
static_assert(is_same_v<int,eval<const_test4_q,int,float>>); // good

template <class F, class G>
struct composey
{
  template <class T>
  using m_invokey = eval<F,eval<G,T>>;
  template <class ...Ts>
  using m_invoke = typename impl::invoke_m<void,m_invokey,Ts...>::type;
};
static_assert(is_same_v<float,eval<composey<id,id>,float>>);
static_assert(is_same_v<int,eval<composey<const_,id>,int,char>>);

using composey_q = quote<composey>;
static_assert(is_same_v<int,eval<composey_q,id,id,int>>);
static_assert(is_same_v<int,eval<composey_q,const_,id,int,char>>);

  template <template <class...> class M>
  struct naive_quote {
    template <class ...Us>
    using m_invoke = M<Us...>;
  };
#endif
#ifdef CURTAINS_N
template <class T>
struct const_test4 {
  template <class U>
  using m_invoke = T;
  using type = bases<quote<m_invoke>,ic<1>>; // Alas, bases is pollution here
};
using const_test4_q = quote_c<const_test4>;
using const_test4_n = bases<const_test4_q,ic<1>>;
static_assert(is_same_v<int,eval<const_test4_n,int,float>>);
#endif

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
static_assert(is_same_v<
                ic<-6>,
                eval<foldl,subtract,ic<0>,list<ic<1>,ic<2>,ic<3>>>
              >);

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

using sum = eval<foldr,add,ic<0>>;
static_assert(is_same_v<ic<6>,eval<sum,list<ic<1>,ic<2>,ic<3>>>>);

// myreverse     xs = foldr f id xs []
//                     where f x y = y . ((:)    x)

// pf \x y -> y . ((:)    x)
// flip (.) . (:)  ...refactor to remove operators:
// (.) (flip (.))  (:)
template <typename T>
using reverse = eval<
                  foldr,
                  eval<compose,eval<flip,compose>,cons>,
                  id,
                  T,
                  list<>
                >;

// $ pf let f x y = y . ((:) x) in \z -> foldr f id z []
// flip (foldr (flip (.) . (:)) id) []
using reverse_ = eval<
                   flip,
                   eval<foldr,eval<compose,eval<flip,compose>,cons>,id>,
                   list<>
                 >;
static_assert(is_same_v<
                list<char,float>,
                eval<reverse_, list<float,char>>
              >);

template <class reverse>
struct reverse_tests : ic<true> {
  static_assert(is_same_v<list<>, eval<reverse,list<>>>);
  static_assert(is_same_v<list<char>, eval<reverse,list<char>>>);
  static_assert(is_same_v<list<char,float>, eval<reverse,list<float,char>>>);
};

static_assert(reverse_tests<reverse_>::value);
#ifndef CURTAINS_N
static_assert(reverse_tests<quote<reverse>>::value);
#else
static_assert(reverse_tests<bases<quote<reverse>,ic<1>>>::value);
#endif

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

// foldl f v xs = foldr (\x g -> (\a -> g (f a x))) id xs v

// pf "\x g -> (\a -> g (f a x))"
// flip (.) . flip f

// pf "\f v xs -> foldr (\x g -> (\a -> g (f a x))) id xs v"
// flip . flip foldr id . (flip (.) .) . flip
// which is (by hand) ...
// (.) flip ((.) (flip foldr id) ((.) (flip (.) .) flip))
// which is (by hand) ...
// (.) flip ((.) (flip foldr id) ((.) ((.) (flip (.))) flip))
using  fldl = eval<
                compose,
                flip,
                eval<
                  compose,
                  eval<flip,foldr,id>,
                  eval<compose,eval<compose,eval<flip,compose>>,flip>
                >
              >;
static_assert(is_same_v<
                ic<-6>,
                eval<fldl,subtract,ic<0>,list<ic<1>,ic<2>,ic<3>>>
              >);

template <class F, class V, class XS>
struct fldl2 {
  template <class X, class G>
  struct s1 {
    template <class A>
    struct s2 {
      using type = eval<G,eval<F,A,X>>;
    };
    using type = quote_c<s2>;
  };
  using type = eval<foldr,quote_c<s1>,id,XS,V>;
};

#ifndef CURTAINS_N
static_assert(is_same_v<
                ic<-6>,
                eval<quote_c<fldl2>,subtract,ic<0>,list<ic<1>,ic<2>,ic<3>>>
              >);
#endif // CURTAINS_N

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
static_assert(is_same_v<
                list<void,void>,
                eval<map,eval<const_,void>,list<int,double>>
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

// all
static_assert(true  == eval<all,odd,list<ic<1>,ic<3>,ic<5>,ic<7>>>{});
#ifndef CURTAINS_N
static_assert(true  == eval<quote_c<std::is_void>,void>{});
static_assert(false == eval<all,quote_c<std::is_void>,list<int,void>>{});
#endif

// fix
static_assert(typename impl::fix_fact_c<int, ic<0>>::type{} == 1);
static_assert(typename impl::fix_fact_c<char,ic<0>>::type{} == 1);
static_assert(typename impl::fix_fact_c<id,ic<1>>::type{} == 0);  // 1 * 0
static_assert(typename impl::fix_fact_c<id,ic<7>>::type{} == 42); // 7 * 6

/*

  While Haskell is happy with a unary definition of fix:
  let fix f = f (fix f)
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

#ifndef CURTAINS_N
// functor and monad instance for ((->) r)
using fmap_reader   = compose;
using return_reader = const_;
template <class M, class X>
using join_reader_t = eval<M,X,X>;
using join_reader = quote<join_reader_t>;
template <class M, class F>
using bind_reader_t = eval<join_reader,eval<fmap_reader,F,M>>;
using bind_reader = quote<bind_reader_t>;

static_assert(is_same_v<ic<4>,eval<join_reader,add,ic<2>>>);
static_assert(is_same_v<id,eval<join_reader,id,id>>);
static_assert(is_same_v<
                int,
                eval<eval<bind_reader,eval<const_,int>,const_>,char>
              >);
static_assert(is_same_v<
                ic<9>,
                eval<eval<bind_reader,eval<mul,ic<2>>,add>,ic<3>>
              >);

static_assert(eval<y,fix_fact,ic<3>>{} == 6);

#endif

// dollar ($)
static_assert(is_same_v<double,eval<dollar,id,double>>);
static_assert(is_same_v<double,eval<dollar,id,id,double>>);
static_assert(is_same_v<ic<1>,eval<eval<flip,dollar,ic<0>>,succ>>);
// static_assert( 2 == eval<dollar,succ,succ,ic<0>>::value); // see dollar.hpp

// lazy_if_
static_assert(is_same_v<int,  eval<lazy_if_,ic<true>, list<int>,list<float>>>);
static_assert(is_same_v<float,eval<lazy_if_,ic<false>,list<int>,list<float>>>);
static_assert(is_same_v<int,eval<
                              lazy_if_,
                              std::true_type,
                              list<int>,
                              list<rec_err,ic<3>>
                            >>);
static_assert(is_same_v<int,eval<
                              lazy_if_,
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
static_assert( true  == eval<odd,ic<7>>::value );
static_assert( false == eval<even,ic<7>>{} );
static_assert( true  == eval<gt,ic<2>,ic<1>>{} );
static_assert( false == eval<lt,ic<2>,ic<1>>{} );
static_assert( true  == eval<ge,ic<2>,ic<1>>{} );
static_assert( true  == eval<ge,ic<2>,ic<2>>{} );
static_assert( true  == eval<le,ic<2>,ic<2>>{} );
static_assert( true  == eval<le,ic<1>,ic<2>>{} );
static_assert( true  == eval<eval<le,ic<1>>,ic<2>>{} );
static_assert( 2     == eval<max,ic<1>,ic<2>>{} );
static_assert( 1     == eval<min,ic<1>,ic<2>>{} );

// logical
static_assert(is_same_v<ic<true>, eval<not_,ic<false>>>);
static_assert(is_same_v<ic<false>,eval<and_,ic<false>,ic<true>>>);
static_assert(is_same_v<ic<true>, eval<and_,ic<true>, ic<true>>>);
static_assert(is_same_v<ic<true>, eval<or_, ic<false>,ic<true>>>);
static_assert(is_same_v<ic<false>,eval<or_, ic<false>,ic<false>>>);

using liftAF = eval<compose,S,const_>;   // liftA for ((->)r)
static_assert(is_same_v<ic<3>,eval<liftAF,succ,succ,ic<1>>>);

template <class F, class T, class U>
using liftAF2_t = eval<foldl,S,eval<const_,F>,list<T,U>>;
using liftAF2 = quote<liftAF2_t>;

#ifndef CURTAINS_N
// Variable arity version of liftA for functions. Requires a list for now.
template <class, class> struct liftAFN_c;
template <class F, class ...Ts>
struct liftAFN_c<F,list<Ts...>>
     : impl::id_c<eval<foldl,S,eval<const_,F>,list<Ts...>>> {};
using liftAFN = quote_c<liftAFN_c>;

static_assert(is_same_v<ic<2>,eval<const_,succ,void,ic<1>>>);
static_assert(is_same_v<ic<3>,eval<S,eval<const_,succ>,succ,ic<1>>>);
static_assert(is_same_v<
                ic<2>,
                eval<eval<foldl,S,eval<const_,succ>,list<>>,void,ic<1>>
              >);
// For ((->)r): pure ≡ const and also (<*>) ≡ S
// pure succ <*> succ  ≡ const succ `S` succ
static_assert(is_same_v<
                ic<3>,
                eval<eval<foldl,S,eval<const_,succ>,list<succ>>,ic<1>>
              >);
static_assert(is_same_v<
                ic<3>,
                eval<foldl,S,eval<const_,succ>,list<succ>,ic<1>>
              >);
static_assert(3 == eval<liftAF,succ,succ,ic<1>>{});
static_assert(3 == eval<liftAFN,succ,list<succ>,ic<1>>{});

// pure not <*> (==1)  ≡ const not `S` (==1)
static_assert(is_same_v<
                ic<false>,
                eval<foldl,S,eval<const_,not_>,list<eval<eq,ic<1>>>,ic<1>>
              >);
static_assert(false == eval<liftAFN,not_,list<eval<eq,ic<1>>>,ic<1>>{} );

// pure (&&) <*> (>1) <*> odd ≡ const (&&) `S` (>1) `S` odd
static_assert(is_same_v<
                ic<true>,
                eval<
                  foldl,
                  S,
                  eval<const_,and_>,
                  list<eval<flip,gt,ic<1>>,odd>,
                  ic<3>
                >
              >);
static_assert(true == eval<liftAF2,and_,eval<flip,gt,ic<1>>,odd,ic<3>>{});
static_assert(true == eval<liftAFN,and_,list<eval<flip,gt,ic<1>>,odd>,ic<3>>{});

// pure max <*> succ <*> pred ≡ const max `S` succ `S` pred
static_assert(is_same_v<
                ic<4>,
                eval<foldl,S,eval<const_,max>,list<succ,pred>,ic<3>>
              >);
static_assert(4 == eval<liftAF2,max,succ,pred,ic<3>>{});
static_assert(4 == eval<liftAFN,max,list<succ,pred>,ic<3>>{});

// Prelude Control.Applicative> let if' c x y = if c then x else y
// Prelude Control.Applicative> pure if' <*> odd <*> id <*> succ $ 7
static_assert(8==eval<liftAFN,if_,list<odd,succ,id>,ic<7>>{});

#endif

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
// Prelude> :t const const
// const const :: b1 -> a -> b -> a
// curtains thinks the arity is 1:
static_assert(is_same_v<ic<1>,eval<arity,eval<const_,const_>>>);
// ...but, it is! or can be seen so...
// as in... b1 -> a -> b -> a is a unary function returning a binary function.
// Consider compose: in compose.hpp we define its arity to 3, but it could
// just as well be understood as having an arity of 2.
// At least Haskell's type checker can give a consistent arity value:
//   the number of arrows which are not contained within parentheses.

static_assert(is_same_v<ic<1>,eval<arity,eval<compose,id,id>>>);
static_assert(is_same_v<ic<2>,eval<arity,eval<compose,id,id,const_>>>);
static_assert(is_same_v<ic<3>,eval<arity,eval<compose,id,id,const3>>>);
static_assert(is_same_v<ic<2>,eval<arity,eval<const_,const_,void>>>);
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

#ifndef CURTAINS_V_SIMPLE
#ifdef CURTAINS_V

// zero_constv
static_assert(is_same_v<const_,eval<zero_constv>>);
static_assert(is_same_v<const_,eval<zero_constv,int,bool>>);

// zero_const
static_assert(is_same_v<const_,eval<zero_const>>);
static_assert(is_same_v<int,eval<zero_const,int,bool>>);

#endif // CURTAINS_V
#endif // CURTAINS_V_SIMPLE

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
