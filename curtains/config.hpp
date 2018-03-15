#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#ifdef CURTAINS_N

using namespace curtains::n;

#else

#ifndef   CURTAINS_V
#define   CURTAINS_V
#endif // CURTAINS_V

using namespace curtains::v; // default

#endif

#endif // __CONFIG_HPP__
