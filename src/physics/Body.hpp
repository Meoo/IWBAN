/**
 * @file   Body.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BODY_HPP_
#define _IWBAN_BODY_HPP_

#include <Global.hpp>

#include <physics/CollisionData.hpp>
#include <physics/CollisionGroup.hpp>

#include <utils/Rectangle.hpp>
#include <utils/Vector.hpp>

#include <boost/intrusive/list.hpp>

#ifndef NDEBUG
#  include <graphics/contexts/debug/DebugContext.hpp>
#endif

namespace phy
{

class Body;
class Shape;

namespace impl
{
    // Use intrusive linked lists for child list
    class ChildTag {};
    typedef boost::intrusive::list_base_hook<
        boost::intrusive::base_hook<ChildTag> >     ChildHook;
    typedef boost::intrusive::list<Body,
        boost::intrusive::base_hook<ChildHook> >    ChildList;
}
// namespace impl

// TODO Boost's hooks must use public inheritance?
class Body : public impl::ChildHook
{
public:
    typedef impl::ChildList     ChildList;

    class State
    {
    public:
        ut::Vector  position;
        ut::Vector  velocity;

    };
    // class State


private:
    // Data members
    const Shape *   _shape;

    Body *          _parent         = nullptr;
    ChildList       _childs;

    unsigned        _active_state   = 0;
    State           _states[2];

    ut::Vector      _acceleration;

    // Mass <= 0 means infinite (cannot be moved by other bodies)
    float           _mass           = 0;

    CollisionGroup  _solidity_group = COL_NONE;
    CollisionGroup  _collision_mask = COL_NONE;

    bool            _awake          = true;

    // TODO Object additional states
    //bool just_teleported;
    //bool smooth;


public:
    // Constructor
    explicit        Body(const Shape * shape);

    // Functions
    void            move(const ut::Vector & delta);
    void            moveTo(const ut::Vector & position);

    void            step();

    // TODO Add wake everytime any property is modified
    // Getters / setters
    const ut::Vector & getPosition() const          { return _states[_active_state].position; }
    const ut::Vector & getVelocity() const          { return _states[_active_state].velocity; }

    const Shape *   getShape() const                { return _shape; }
    void            setShape(const Shape * shape)   { IWBAN_PRE_PTR(shape); _shape = shape; }

    Body *          getParent()                     { return _parent; }
    const Body *    getParent() const               { return _parent; }

    // TODO Handle child list too, and wake
    void            setParent(Body * parent)        { _parent = parent; }

    float           getMass() const                 { return _mass; }
    void            setMass(float mass)             { _mass = mass; }

    CollisionGroup  getSolidityGroup() const                { return _solidity_group; }
    void            setSolidityGroup(CollisionGroup group)  { _solidity_group = group; }

    CollisionGroup  getCollisionMask() const                { return _collision_mask; }
    void            setCollisionMask(CollisionGroup mask)   { _collision_mask = mask; }

    bool            isAwake() const                 { return _awake; }
    void            sleep()                         { _awake = false; }
    void            wake()                          { _awake = true; }

    ut::Rectangle   getBoundingBox() const;


#ifndef NDEBUG
    void            drawDebug(gfx::DebugContext & debug_context) const;
#endif

    // Static functions
    /**
     * Check if two bodies can collide.
     *
     * Use this function to check roughly if two objects can collide before
     * using #collide to perform a more precise collision detection.
     */
    static bool     canCollide(const Body & first, const Body & second);

    /**
     * Perform fine collision detection.
     *
     * You should call #canCollide on the two bodies first.
     */
    static bool     collide(const Body & first, const Body & second, CollisionData & data);

};
// class Object

}
// namespace phy

#endif // _IWBAN_BODY_HPP_
