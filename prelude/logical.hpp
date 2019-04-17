#ifndef __LOGICAL_HPP__
#define __LOGICAL_HPP__

#include "../curtains.hpp"

namespace curtains::impl {

  template <class>        struct not_c;
  template <class, class> struct and_c;
  template <class, class> struct or_c;

} // namespace curtains::impl

namespace curtains::v {

  using not_ = quote_c<impl::not_c>;
  using and_ = quote_c<impl::and_c>;
  using or_  = quote_c<impl::or_c>;

} // namespace curtains::v

namespace curtains::n {

  using not_ = bases<v::not_,ic<1>>;
  using and_ = bases<v::and_,ic<2>>;
  using or_  = bases<v::or_, ic<2>>;

} // namespace curtains::v

namespace curtains::impl {

  template <auto B>         struct not_c<ic<B>>             : id_c<ic<!B>>   {};
  template <auto A, auto B> struct and_c<ic<A>,ic<B>>       : id_c<ic<A&&B>> {};
  template <auto A, auto B> struct  or_c<ic<A>,ic<B>>       : id_c<ic<A||B>> {};

} // namespace curtains::impl

#endif //  __LOGICAL_HPP__
