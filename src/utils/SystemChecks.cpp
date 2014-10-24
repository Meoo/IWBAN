/**
 * @file   SystemChecks.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <SFML/Config.hpp>

#include <boost/version.hpp>

#include <lua.h>

#include <limits>

// Ensure that IEEE754 is in use
IWBAN_STATIC_ASSERT(std::numeric_limits<float>::is_iec559);

// Ensure that Lua version is 5.2
IWBAN_STATIC_ASSERT(LUA_VERSION_NUM == 502);

// Ensure that Boost version is 1.55
IWBAN_STATIC_ASSERT(BOOST_VERSION / 100 == 1055);

// Ensure that SFML version is 2.1
IWBAN_STATIC_ASSERT(SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR == 1);
