/**
 * @file   MovingBehavior.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/Object.hpp>
#include <physics/behaviors/MovingBehavior.hpp>

namespace phy
{

void MovingBehavior::step(Object & object)
{
    object.setVelocity(object.getPosition() - object.getLastPosition());
}

void MovingBehavior::stepChild(const Object & object, Object & child)
{
    child.move(object.getVelocity());
}

}
// namespace phy

