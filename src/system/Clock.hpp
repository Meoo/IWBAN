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

inline
const sf::Clock & getGlobalClock()
{
    static sf::Clock s_global_clock;
    return s_global_clock;
}

}
// namespace sys

#endif // _IWBAN_CLOCK_HPP_
