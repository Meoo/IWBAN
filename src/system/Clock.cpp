/**
 * @file   Clock.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <system/Clock.hpp>

namespace sys
{

namespace impl
{

sf::Clock global_clock;

sf::Time update_time;
sf::Time draw_time;

}
// namespace impl

}
// namespace sys
