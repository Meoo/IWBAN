/**
 * @file   CollisionData.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_COLLISIONDATA_HPP_
#define _IWBAN_COLLISIONDATA_HPP_

#include <Global.hpp>

#include <physics/CollisionGroup.hpp>

#include <utils/Vector.hpp>

namespace phy
{

class CollisionData
{
public:
    CollisionGroup  first_mask;
    CollisionGroup  second_mask;

    ut::Vector      origin;
    ut::Vector      mtv;    // Minimum Translation Vector

};
// class CollisionData

}
// namespace phy

#endif // _IWBAN_COLLISIONDATA_HPP_
