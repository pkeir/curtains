#ifndef __SWAP__HPP__
#define __SWAP__HPP__

template <class...>
  struct lazy {};

namespace curtains::impl {

  template <class, class, class T, class = void>
    struct swap_c;

  template <class F, class ...Ts>
  using swap_t = typename swap_c<F,Ts...>::type;

} // namespace curtains::impl

namespace curtains::v {

  using swap = quote_c<impl::swap_c>;

} // namespace curtains::v

namespace curtains::n {

  using swap = bases<v::swap,ic<3>>;

} // namespace curtains::n

namespace curtains::impl {

  template <class, class, class T, class>
  struct swap_c : id_c<T> {};

  template <class F, class G, template <class> class TT, class ...Ts>
  struct swap_c<F,G,TT<Ts...>,
    std::enable_if_t<std::is_same_v<eval<F,Ts...>,TT<Ts...>>>>
  {
    using type = eval<G,eval<swap,F,G,Ts>...>;
  };

} // namespace curtains::impl

#endif // __SWAP__HPP__
