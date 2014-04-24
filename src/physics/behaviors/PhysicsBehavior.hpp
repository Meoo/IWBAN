/**
 * @file   PhysicsBehavior.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_PHYSICSBEHAVIOR_HPP_
#define _IWBAN_PHYSICSBEHAVIOR_HPP_

#include <Global.hpp>

#include <physics/Behavior.hpp>

#include <utils/Vector.hpp>

namespace phy
{

class PhysicsBehavior : public phy::Behavior
{
private:
    // Data members
    ut::Vector  _gravity;
    ut::Vector  _speed_clamp;


public:
    // Constructor
                    PhysicsBehavior();

    // Virtual functions
    virtual void    step(Object & object);

    virtual void    stepChild(const Object & object, Object & child);

    virtual void    onCollide(Object & object, Object & other, const CollisionData & data);

    virtual void    prepare(Object &) {}

    virtual void    free(Object &) {}

};
// class PhysicsBehavior

}
// namespace phy

#endif // _IWBAN_PHYSICSBEHAVIOR_HPP_
