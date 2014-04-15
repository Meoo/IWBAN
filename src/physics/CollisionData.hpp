/**
 * @file   CollisionData.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_COLLISIONDATA_HPP_
#define _IWBAN_COLLISIONDATA_HPP_

#include <Global.hpp>

#include <utils/Vector.hpp>

namespace phy
{

class Object;

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,

};
// enum Direction

/**
 * @brief
 */
class CollisionData
{
public:
    Object *    first;
    Object *    second;

    ut::Vector  origin;
    Direction   direction;
    float       force;

};
// class CollisionData

}
// namespace phy

#endif // _IWBAN_COLLISIONDATA_HPP_
