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

const CollisionGroup COL_WORLD      = 0x01;
const CollisionGroup COL_PLAYER     = 0x02;
const CollisionGroup COL_ENEMY      = 0x04;

const CollisionGroup COL_TRAP       = 0x10;
const CollisionGroup COL_TRIGGER    = 0x20;
const CollisionGroup COL_SPECIAL    = 0x40;

const CollisionGroup COL_NONE       = 0x00;
const CollisionGroup COL_ALL        = 0xFF;


#ifndef NDEBUG
inline
sf::Color getColorFromCollisionGroup(CollisionGroup group)
{
    if (group & COL_SPECIAL)
        return sf::Color::Cyan;

    if (group & COL_PLAYER)
        return sf::Color::Green;

    if (group & (COL_ENEMY | COL_TRAP))
        return sf::Color::Red;

    if (group & COL_TRIGGER)
        return sf::Color::Magenta;

    if (group & COL_WORLD)
        return sf::Color::Blue;

    return sf::Color::Yellow;
}
#endif

}
// namespace phy

#endif // _IWBAN_COLLISIONGROUP_HPP_
