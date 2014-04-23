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

class CollisionData
{
public:
    ut::Vector  origin;
    ut::Vector  intersect;

    void revert()
    {
        intersect.x = - intersect.x;
        intersect.y = - intersect.y;
    }

};
// class CollisionData

}
// namespace phy

#endif // _IWBAN_COLLISIONDATA_HPP_
