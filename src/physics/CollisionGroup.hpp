/**
 * @file   CollisionGroup.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_COLLISIONGROUP_HPP_
#define _IWBAN_COLLISIONGROUP_HPP_

#include <Global.hpp>

#ifndef NDEBUG
#  include <SFML/Graphics.hpp>
#endif

namespace phy
{

enum CollisionGroup
{
    WORLD       = 0x01,
    PLAYER      = 0x02,
    ENEMY       = 0x04,

    TRAP        = 0x10,
    TRIGGER     = 0x20,
    SPECIAL     = 0x40,

    NONE        = 0x00,
    ALL         = 0xFF,
};
// enum CollisionGroup

#ifndef NDEBUG
inline
sf::Color getColorFromCollisionGroup(CollisionGroup group)
{
    if (group & SPECIAL)
        return sf::Color::Cyan;

    if (group & PLAYER)
        return sf::Color::Green;

    if (group & (ENEMY | TRAP))
        return sf::Color::Red;

    if (group & TRIGGER)
        return sf::Color::Magenta;

    if (group & WORLD)
        return sf::Color::Blue;

    return sf::Color::Yellow;
}
#endif

}
// namespace phy

#endif // _IWBAN_COLLISIONGROUP_HPP_
