/**
 * @file   Global.hpp
 * @author Bastien Brunnenstein
 *
 * Main header.
 */

#ifndef _IWBAN_GLOBAL_HPP_
#define _IWBAN_GLOBAL_HPP_

#include <boost/assert.hpp>

#include <cstdio>


#define _IWBAN_DO(action)   do { action; } while(0)

#define _IWBAN_LOG_FILE     stderr

#define _IWBAN_PRINT_LOG(...)                               \
    _IWBAN_DO(std::fprintf(_IWBAN_LOG_FILE, __VA_ARGS__);   \
              std::fprintf(_IWBAN_LOG_FILE, "\n");          \
              std::fflush(_IWBAN_LOG_FILE))

/**
 * @def IWBAN_LOG_LEVEL
 * Log level, higher means more logs.
 *
 * 0 : Critical (Min)
 * 1 : Error
 * 2 : Warning
 * 3 : Info
 * 4 : Debug (Max)
 */
#ifndef IWBAN_LOG_LEVEL
#  ifdef NDEBUG
#    define IWBAN_LOG_LEVEL 2
#  else
#    define IWBAN_LOG_LEVEL 4
#  endif
#endif

/**
 * @def IWBAN_LOG_CRITICAL
 * @def IWBAN_LOG_ERROR
 * @def IWBAN_LOG_WARNING
 * @def IWBAN_LOG_INFO
 * @def IWBAN_LOG_DEBUG
 * Log functions.
 */

#define IWBAN_LOG_CRITICAL(...)  _IWBAN_PRINT_LOG("CRIT : " __VA_ARGS__)

#if IWBAN_LOG_LEVEL >= 1
#  define IWBAN_LOG_ERROR(...)   _IWBAN_PRINT_LOG("ERROR: " __VA_ARGS__)
#else
#  define IWBAN_LOG_ERROR(...)   _IWBAN_DO()
#endif

#if IWBAN_LOG_LEVEL >= 2
#  define IWBAN_LOG_WARNING(...) _IWBAN_PRINT_LOG("WARN : " __VA_ARGS__)
#else
#  define IWBAN_LOG_WARNING(...) _IWBAN_DO()
#endif

#if IWBAN_LOG_LEVEL >= 3
#  define IWBAN_LOG_INFO(...)    _IWBAN_PRINT_LOG("Info : " __VA_ARGS__)
#else
#  define IWBAN_LOG_INFO(...)    _IWBAN_DO()
#endif

#if IWBAN_LOG_LEVEL >= 4
#  define IWBAN_LOG_DEBUG(...)   _IWBAN_PRINT_LOG("Debug: " __VA_ARGS__)
#else
#  define IWBAN_LOG_DEBUG(...)   _IWBAN_DO()
#endif

// ---- ---- ---- ----

/**
 * @def IWBAN_DEBUG
 * Macro to enable a statement in Debug mode only.
 */
#ifdef NDEBUG
#  define IWBAN_DEBUG(x)
#else
#  define IWBAN_DEBUG(x) x
#endif

// ---- ---- ---- ----

// Precondition asserts
#define IWBAN_PRE(x)                    BOOST_ASSERT(x)
#define IWBAN_PRE_PTR(x)                BOOST_ASSERT((x) != nullptr)
#define IWBAN_PRE_RANGE(x, min, max)    BOOST_ASSERT((x) >= min && (x) < max)
#define IWBAN_PRE_MSG(x, msg)           BOOST_ASSERT_MSG(x, msg)

// Normal asserts
#define IWBAN_ASSERT(x)                 BOOST_ASSERT(x)
#define IWBAN_ASSERT_MSG(x, msg)        BOOST_ASSERT_MSG(x, msg)

// Assert, and always execute (even in release mode)
#define IWBAN_VERIFY(x)                 BOOST_VERIFY(x)

// Static assert
#define IWBAN_STATIC_ASSERT(x)          static_assert(x, #x)

#endif // _IWBAN_GLOBAL_HPP_
