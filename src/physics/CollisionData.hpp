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

class Body;

class CollisionData
{
public:
    const Body *    first;
    const Body *    second;

    CollisionGroup  first_mask;
    CollisionGroup  second_mask;

    ut::Vector      origin;
    ut::Vector      mtv;    // Minimum Translation Vector

};
// class CollisionData

// ---- ---- ---- ----

class CollisionResult
{
public:
    const Body *    body;
    CollisionGroup  mask;

    ut::Vector      origin;
    ut::Vector      force;
    float           strength;

};
// class CollisionResult

}
// namespace phy

#endif // _IWBAN_COLLISIONDATA_HPP_
