/**
 * @file   Body.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BODY_HPP_
#define _IWBAN_BODY_HPP_

#include <Global.hpp>

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
    class ListTag {};

    typedef boost::intrusive::list_base_hook<
        boost::intrusive::tag<ListTag> > ObjectHook;

    typedef boost::intrusive::list<Body,
        boost::intrusive::base_hook<ObjectHook> > ObjectList;


    // ---- ---- ---- ----

    class ChildTag {};

    // Child-list must support auto-unlink feature ...
    typedef boost::intrusive::list_base_hook<
        boost::intrusive::base_hook<ChildTag>,
        boost::intrusive::link_mode<boost::intrusive::auto_unlink> > ChildHook;

    // ... so constant size = false is required too
    typedef boost::intrusive::list<Body,
        boost::intrusive::base_hook<ChildHook>,
        boost::intrusive::constant_time_size<false> > ChildList;

}
// namespace impl

// Boost's hooks must use public inheritance
class Body : public impl::ObjectHook,
             public impl::ChildHook
{
public:
    typedef impl::ObjectList    List;
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

    bool            _active_state   = false;
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

    // Getters / setters
    const Shape *   getShape() const                { return _shape; }
    void            setShape(const Shape * shape)   { BOOST_ASSERT(shape); _shape = shape; }

    Body *          getParent()                     { return _parent; }
    const Body *    getParent() const               { return _parent; }  // TODO Handle child list too
    void            setParent(Body * parent)        { _parent = parent; }

    float           getMass() const                 { return _mass; }
    void            setMass(float mass)             { _mass = mass; }

    CollisionGroup  getSolidityGroup() const                { return _solidity_group; }
    void            setSolidityGroup(CollisionGroup group)  { _solidity_group = group; }

    CollisionGroup  getCollisionMask() const                { return _collision_mask; }
    void            setCollisionMask(CollisionGroup mask)   { _collision_mask = mask; }

    bool            isAwake() const         { return _awake; }
    void            sleep()                 { _awake = false; }
    void            wake()                  { _awake = true; }

    ut::Rectangle   getBoundingBox() const;


#ifndef NDEBUG
    void            drawDebug(gfx::DebugContext & debug_context) const;
#endif

};
// class Object

}
// namespace phy

#endif // _IWBAN_BODY_HPP_
