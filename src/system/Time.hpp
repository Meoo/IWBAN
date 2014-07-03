/**
 * @file   Time.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_TIME_HPP_
#define _IWBAN_TIME_HPP_

#include <Global.hpp>

#include <config/DisplayConfig.hpp>

#include <SFML/System.hpp>

namespace sys
{

/**
 * Time as a number of frames.
 */
typedef unsigned FTime;

typedef int      FTimeOffset;

constexpr FTimeOffset fseconds(float seconds)
{
    return static_cast<FTimeOffset>(seconds * IWBAN_UPDATE_RATE);
}

// ---- ---- ---- ----

namespace impl
{

extern sf::Clock global_clock;

}
// namespace impl

// ---- ---- ---- ----

/**
 * Get current global time.
 */
inline
sf::Time getGlobalTime()
{
    return impl::global_clock.getElapsedTime();
}

}
// namespace sys

#endif // _IWBAN_TIME_HPP_
