/**
 * @file   CollisionData.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_COLLISIONDATA_HPP_
#define _IWBAN_COLLISIONDATA_HPP_

#include <Global.hpp>

namespace phy
{

/**
 * @brief
 */
class CollisionData
{
    Object  first;
    Object  second;
    Vector2 collision_point;
    Vector2 push_vector;

};
// class CollisionData

}
// namespace phy

#endif // _IWBAN_COLLISIONDATA_HPP_
