/**
 * @file   Behavior.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BEHAVIOR_HPP_
#define _IWBAN_BEHAVIOR_HPP_

#include <Global.hpp>

namespace phy
{

class Behavior
{
public:
    // Virtual destructor
    virtual ~Behavior() {}

    // TODO Virtual functions
    virtual void step(const ObjectState last, ObjectState & current) = 0;
    virtual void onCollide(CollisionData) = 0;
    virtual void prepareState(const Object, ObjectState &) = 0;
    virtual void freeState(const Object, ObjectState &) = 0;

};
// class Behavior

}
// namespace phy

#endif // _IWBAN_BEHAVIOR_HPP_
