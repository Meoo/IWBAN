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

typedef unsigned int CollisionGroup;

const CollisionGroup WORLD      = 0x01;
const CollisionGroup PLAYER     = 0x02;
const CollisionGroup ENEMY      = 0x04;

const CollisionGroup TRAP       = 0x10;
const CollisionGroup TRIGGER    = 0x20;
const CollisionGroup SPECIAL    = 0x40;

const CollisionGroup NONE       = 0x00;
const CollisionGroup ALL        = 0xFF;


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
