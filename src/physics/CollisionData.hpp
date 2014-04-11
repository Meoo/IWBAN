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

/**
 * @brief
 */
class CollisionData
{
public:
    Object *    first;
    Object *    second;
    ut::Vector  collision_point;
    ut::Vector  push_vector;

};
// class CollisionData

}
// namespace phy

#endif // _IWBAN_COLLISIONDATA_HPP_
