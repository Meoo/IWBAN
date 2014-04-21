/**
 * @file   Behavior.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BEHAVIOR_HPP_
#define _IWBAN_BEHAVIOR_HPP_

#include <Global.hpp>

namespace phy
{

class CollisionData;
class Object;

class Behavior
{
public:
    // Virtual destructor
    virtual ~Behavior() {}

    // TODO Virtual functions
    virtual void step(const Object &) = 0;

    // Pre-condition : parent have been updated
    virtual void stepChild(const Object & object, Object & child) = 0;

    virtual void onCollide(Object & object, Object & other,
                           const CollisionData & data) = 0;

    // Callbacks used when an Object starts and stop using this Behavior
    // Used to setup and free user_data pointer
    virtual void prepare(Object &) = 0;
    virtual void free(Object &) = 0;

};
// class Behavior

}
// namespace phy

#endif // _IWBAN_BEHAVIOR_HPP_
