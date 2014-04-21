/**
 * @file   CollisionGroup.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_COLLISIONGROUP_HPP_
#define _IWBAN_COLLISIONGROUP_HPP_

#include <Global.hpp>

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

}
// namespace phy

#endif // _IWBAN_COLLISIONGROUP_HPP_
