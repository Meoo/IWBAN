/**
 * @file   Time.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_TIME_HPP_
#define _IWBAN_TIME_HPP_

#include <Global.hpp>

#include <SFML/System.hpp>

namespace sys
{

namespace impl
{

extern sf::Clock global_clock;

}
// namespace impl

// ---- ---- ---- ----

/**
 * Get current time.
 */
inline
sf::Time getGlobalTime()
{
    return impl::global_clock.getElapsedTime();
}

}
// namespace sys

#endif // _IWBAN_TIME_HPP_
