#ifndef DEFINES_H
#define DEFINES_H
#include <cinttypes>

#define UNUSED(param) (void)param
#define UNIQUE_LOCK(mutex_var) boost::unique_lock<boost::mutex> unique_lock_define(mutex_var); (void)unique_lock_define

#endif // DEFINES_H
