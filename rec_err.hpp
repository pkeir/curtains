#ifndef __REC_ERR_HPP__
#define __REC_ERR_HPP__

#include "curtains.hpp"

// A metafunction class to produce a recursive template instantiation error.
// Something like eval<rec_err,ic<3>> will create such an error. Adding
// the commented specialisation of rec_err_c will remove such error.

namespace curtains::impl {

  template <class> struct rec_err_c;

  template <class T>
  using rec_err_t = typename rec_err_c<T>::type;

} //  namespace curtains::impl

namespace curtains::v {

  using rec_err = quote<impl::rec_err_t>;

} // namespace curtains::v

namespace curtains::n {

  using rec_err = bases<v::rec_err,ic<1>>;

} // namespace curtains::n

namespace curtains::impl {

  // template <> struct rec_err_c<ic<0>> : id_c<ic<0>> {};

  template <auto N>
  struct rec_err_c<ic<N>> : id_c<rec_err_t<ic<N-1>>> {};

} // namespace curtains::impl

#endif //  __REC_ERR_HPP__
