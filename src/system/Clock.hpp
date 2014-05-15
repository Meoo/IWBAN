/**
 * @file   Clock.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CLOCK_HPP_
#define _IWBAN_CLOCK_HPP_

#include <Global.hpp>

#include <SFML/System.hpp>

namespace sys
{

namespace impl
{

extern sf::Clock global_clock;

extern sf::Time update_time;
extern sf::Time draw_time;

inline
void setUpdateTime()
{
    update_time = global_clock.getElapsedTime();
}

inline
void setDrawTime()
{
    draw_time = global_clock.getElapsedTime();
}

}
// namespace impl

/**
 * Get current time.
 */
inline
sf::Time getGlobalTime()
{
    return impl::global_clock.getElapsedTime();
}

/**
 * Get time of last frame update.
 */
inline
const sf::Time & getUpdateTime()
{
    return impl::update_time;
}

/**
 * Get time of last frame draw.
 */
inline
const sf::Time & getDrawTime()
{
    return impl::draw_time;
}

}
// namespace sys

#endif // _IWBAN_CLOCK_HPP_
