/**
 * @file   PhysicsBehavior.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/CollisionData.hpp>
#include <physics/Object.hpp>
#include <physics/behaviors/PhysicsBehavior.hpp>

namespace phy
{

PhysicsBehavior::PhysicsBehavior()
{
    _gravity = ut::Vector(0, 0.3f);
    _speed_clamp = ut::Vector(8, 8);
}

void PhysicsBehavior::step(phy::Object & object)
{
    ut::Vector vel = object.getVelocity();

    // Gravity
    vel += _gravity;

    // Clamping
    if (vel.x > _speed_clamp.x)
        vel.x = _speed_clamp.x;
    else if (vel.x < - _speed_clamp.x)
        vel.x = - _speed_clamp.x;

    if (vel.y > _speed_clamp.y)
        vel.y = _speed_clamp.y;
    else if (vel.y < - _speed_clamp.y)
        vel.y = - _speed_clamp.y;

    object.setVelocity(vel);
    object.move(vel);
}

void PhysicsBehavior::stepChild(const phy::Object & object, phy::Object & child)
{
    child.move(object.getVelocity());
}

void PhysicsBehavior::onCollide(phy::Object & object, phy::Object & other, const phy::CollisionData & data)
{
    ut::Vector delta_vel = other.getVelocity() - object.getVelocity();

    // Split horizontally or vertically?
    if (std::abs(data.intersect.x - delta_vel.x)
      > std::abs(data.intersect.y - delta_vel.y))
    {
        // Vertical
        object.move(ut::Vector(0, data.intersect.y));

        // Stop the object if it collides something
        if ((data.intersect.y > 0) == (delta_vel.y > 0))
        {
            ut::Vector vel = object.getVelocity();
            vel.y = 0;
            object.setVelocity(vel);
        }
    }
    else
    {
        // Horizontal
        object.move(ut::Vector(data.intersect.x, 0));

        // Stop the object if it collides something
        if ((data.intersect.x > 0) == (delta_vel.x > 0))
        {
            ut::Vector vel = object.getVelocity();
            vel.x = 0;
            object.setVelocity(vel);
        }
    }
}

}
// namespace phy

