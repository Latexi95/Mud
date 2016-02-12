#ifndef COMMON_H
#define COMMON_H
#include <cstdint>
typedef std::uint64_t time_type;


//Visual Studio 2013 doesn't support noexcept
#ifdef _MSC_VER
    #if _MSC_VER < 1900
        #define noexcept throw()
    #endif
#endif

#endif // COMMON_H
