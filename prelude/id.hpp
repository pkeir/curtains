#ifndef __ID_HPP__
#define __ID_HPP__

#include "../curtains/curtains_includes.hpp"

namespace curtains::v {

  using id = quote_c<impl::id_c>;

} // namespace curtains::v

namespace curtains::n {

  using id = bases<v::id,ic<1>>;

} // namespace curtains::n

#endif // __ID_HPP__
