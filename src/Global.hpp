/**
 * @file   Global.hpp
 * @author Bastien Brunnenstein
 *
 * Main header.
 */

#ifndef _IWBAN_GLOBAL_HPP_
#define _IWBAN_GLOBAL_HPP_

#include <config/StaticConfig.hpp>

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <string>


#define _IWBAN_DO(action)       do { action; } while(0)

#define _IWBAN_PRINT_ERR(...)                       \
    _IWBAN_DO(std::fprintf(stderr, __VA_ARGS__);    \
              std::fflush(stderr))

/**
 * @def IWBAN_LOG_ERROR
 * @def IWBAN_LOG_WARNING
 * @def IWBAN_LOG_INFO
 * @def IWBAN_LOG_DEBUG
 * Log functions.
 */
#define IWBAN_LOG_ERROR(...)   _IWBAN_PRINT_ERR("ERROR: " __VA_ARGS__)
#define IWBAN_LOG_WARNING(...) _IWBAN_PRINT_ERR("WARN : " __VA_ARGS__)

#ifdef NDEBUG
#  define IWBAN_LOG_INFO(...)  _IWBAN_DO()
#  define IWBAN_LOG_DEBUG(...) _IWBAN_DO()
#else
#  define IWBAN_LOG_INFO(...)  _IWBAN_PRINT_ERR("Info : " __VA_ARGS__)
#  define IWBAN_LOG_DEBUG(...) _IWBAN_PRINT_ERR("Debug: " __VA_ARGS__)
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

#endif // _IWBAN_GLOBAL_HPP_
