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

    BOOST_ASSERT_MSG(_gravity.x == 0 || _gravity.y == 0,
                     "Only one direction should be used for gravity");
}

void PhysicsBehavior::prepare(Object & object)
{
    object.setUserData((void*) 0);
}

void PhysicsBehavior::step(Object & object)
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

void PhysicsBehavior::finish(Object & object)
{
    if (object.getUserData() == (void*) 0)
        object.unsetParent();
}

void PhysicsBehavior::stepChild(const Object & object, Object & child)
{
    child.move(object.getVelocity());
}

void PhysicsBehavior::onCollide(Object & object, Object & other, const CollisionData & data)
{
    // Do not collide with childrens
    if (other.hasParent() && &(other.getParent()) == &object)
        return;

    float mass_factor;

    if (object.getMass() <= 0)
        mass_factor = 0;

    else if (other.getMass() <= 0
            // If the collision is with the parent, ignore mass
            || (object.hasParent() && (&object.getParent() == &other)))
        mass_factor = 1;

    else
        mass_factor = object.getMass() / (object.getMass() + other.getMass());

    ut::Vector delta_vel = other.getVelocity() - object.getVelocity();

    // Split horizontally or vertically?
    if (std::abs(data.intersect.x - delta_vel.x)
      > std::abs(data.intersect.y - delta_vel.y))
    {
        // Vertical
        if (mass_factor > 0)
            object.move(ut::Vector(0, data.intersect.y * mass_factor));

        // Stop the object if it collides something
        if ((data.intersect.y > 0) == (delta_vel.y > 0))
        {
            ut::Vector vel = object.getVelocity();

            if (vel.y != 0 && ((vel.y > 0) != (data.intersect.y > 0)))
                vel.y = 0;

            object.setVelocity(vel);
        }

        // Set the object as parent if the collision is opposing the gravity
        if ((_gravity.y != 0) && ((_gravity.y > 0) != (data.intersect.y > 0)))
        {
            if (object.hasParent())
                object.unsetParent();

            object.setParent(other);
            object.setUserData((void*) 1);
        }
    }
    else
    {
        // Horizontal
        if (mass_factor > 0)
            object.move(ut::Vector(data.intersect.x * mass_factor, 0));

        // Stop the object if it collides something
        if ((data.intersect.x > 0) == (delta_vel.x > 0))
        {
            ut::Vector vel = object.getVelocity();

            if (vel.x != 0 && ((vel.x > 0) != (data.intersect.x > 0)))
                vel.x = 0;

            object.setVelocity(vel);
        }

        // Set the object as parent if the collision is opposing the gravity
        if ((_gravity.x != 0) && ((_gravity.x > 0) != (data.intersect.x > 0)))
        {
            if (object.hasParent())
                object.unsetParent();

            object.setParent(other);
            object.setUserData((void*) 1);
        }
    }
}

}
// namespace phy
