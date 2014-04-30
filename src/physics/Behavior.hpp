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
    virtual void prepare(Object & object) {}

    // Pre-condition : parent have been updated
    virtual void step(Object & object) = 0;

    virtual void finish(Object & object) {}

    // Pre-condition : parent have been updated
    virtual void stepChild(const Object & object, Object & child) = 0;

    virtual void onCollide(Object & object, Object & other,
                           const CollisionData & data) = 0;

    // Callbacks used when an Object starts and stop using this Behavior
    // Used to setup and free user_data pointer
    // TODO onAttach onDetach ?
    //virtual void init(Object &)     {}
    //virtual void free(Object &)     {}

};
// class Behavior

}
// namespace phy

#endif // _IWBAN_BEHAVIOR_HPP_
