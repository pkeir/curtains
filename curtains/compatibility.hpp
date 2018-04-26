#ifndef __COMPATIBILITY_HPP__
#define __COMPATIBILITY_HPP__

//#include <type_traits>

#ifdef __clang__
/*namespace std {
template <class T, class U>
constexpr auto is_same_v = std::is_same<T,U>::value;
}*/
#endif

// Clang still has a problem with std::void_t, and the old fix can still be
// used now in 2017:
// https://bugs.llvm.org/show_bug.cgi?id=33655
#ifdef __clang__
namespace curtains {
template <class ...Ts> struct make_void { typedef void type; };
template <class ...Ts> using void_t = typename make_void<Ts...>::type;
}
#else
namespace curtains {
using std::void_t;
}
#endif

#endif // __COMPATIBILITY_HPP__
