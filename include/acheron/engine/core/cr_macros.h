#ifndef AR_CR_MACROS_H
#define AR_CR_MACROS_H

#if defined(__linux__) || defined(__APPLE__)
#define AR_OS_POSIX
#elif defined(_WIN32)
#define AR_OS_WINDOWS
#endif

#endif
