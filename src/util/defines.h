#ifndef DEFINES_H
#define DEFINES_H
#include <cstdint>

#define UNUSED(param) (void)param
#define UNIQUE_LOCK(mutex_var) boost::unique_lock<boost::mutex> unique_lock_define(mutex_var); (void)unique_lock_define

#ifdef __GNUC__
# define thread_local __thread
#elif defined(_MSC_VER)
# define thread_local __declspec(thread)
#endif
typedef std::uint64_t time_type;


//Visual Studio 2013 doesn't support noexcept
#ifdef _MSC_VER
    #if _MSC_VER < 1900
        #define noexcept throw()
    #endif
#endif


#endif // DEFINES_H
