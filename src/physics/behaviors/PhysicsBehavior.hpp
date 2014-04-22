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
    virtual void    step(phy::Object & object);

    virtual void    stepChild(const phy::Object & object, phy::Object & child);

    virtual void    onCollide(phy::Object & object, phy::Object & other, const phy::CollisionData & data);

    virtual void    prepare(phy::Object &) {}

    virtual void    free(phy::Object &) {}

};
// class PhysicsBehavior

}
// namespace phy

#endif // _IWBAN_PHYSICSBEHAVIOR_HPP_
