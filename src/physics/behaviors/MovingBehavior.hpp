/**
 * @file   MovingBehavior.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MOVINGBEHAVIOR_HPP_
#define _IWBAN_MOVINGBEHAVIOR_HPP_

#include <Global.hpp>

#include <physics/Behavior.hpp>

#include <utils/Vector.hpp>

namespace phy
{

class MovingBehavior : public phy::Behavior
{
public:
    // Virtual functions
    virtual void    step(Object & object);

    virtual void    stepChild(const Object & object, Object & child);

    virtual void    onCollide(Object & object, Object & other, const CollisionData & data) {}

    virtual void    prepare(Object &) {}

    virtual void    free(Object &) {}

};
// class MovingBehavior

}
// namespace phy

#endif // _IWBAN_MOVINGBEHAVIOR_HPP_
