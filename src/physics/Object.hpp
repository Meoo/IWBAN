/**
 * @file   Object.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OBJECT_HPP_
#define _IWBAN_OBJECT_HPP_

#include <Global.hpp>

#include <physics/CollisionGroup.hpp>

#include <utils/Rectangle.hpp>
#include <utils/Vector.hpp>

#include <boost/intrusive/list.hpp>

#ifndef NDEBUG
#  include <graphics/contexts/DrawContext.hpp>
#endif

namespace phy
{

class Behavior;
class Object;
class Shape;
class Simulator;

namespace impl
{
    class ObjectTag {};

    typedef boost::intrusive::list_base_hook<
        boost::intrusive::tag<ObjectTag> > ObjectHook;

    typedef boost::intrusive::list<Object,
        boost::intrusive::base_hook<ObjectHook> > ObjectList;


    // ---- ---- ---- ----

    class ChildTag {};

    // Child-list must support auto-unlink feature ...
    typedef boost::intrusive::list_base_hook<
        boost::intrusive::base_hook<ChildTag>,
        boost::intrusive::link_mode<boost::intrusive::auto_unlink> > ChildHook;

    // ... so constant size = false is required too
    typedef boost::intrusive::list<Object,
        boost::intrusive::base_hook<ChildHook>,
        boost::intrusive::constant_time_size<false> > ChildList;

}
// namespace impl

// TODO Public inheritance? Seems to be required for boost
class Object : public impl::ObjectHook,
               public impl::ChildHook
{
public:
    class Comparator;

    typedef impl::ObjectList    List;
    typedef impl::ChildList     ChildList;


private:
    // Data members
    const Shape *   _shape;

    ChildList       _childs;

    Behavior *      _behavior;
    void *          _user_data;

    ut::Vector      _position;
    ut::Vector      _last_position;

    ut::Vector      _velocity;

    float           _mass;

    CollisionGroup  _solidity;
    CollisionGroup  _collides_with;

    int             _collision_priority;
    bool            _awake;

    // TODO Object additional states
    //bool just_teleported;
    //bool smooth;


public:
    // Constructor
    explicit Object(const Shape * shape, Behavior * behavior = 0);

    // Functions
    void    move(const ut::Vector & delta);

    void    step();

    void    collideWith(Object & other);

    void    setParent(Object & parent);
    void    unsetParent();
    // TODO getParent()? not required?

    // Getters / setters
    const Shape * getShape() const              { return _shape; }
    void          setShape(const Shape * shape) { BOOST_ASSERT(shape); _shape = shape; }

    Behavior * getBehavior() const              { return _behavior; }
    void       setBehavior(Behavior * behavior) { _behavior = behavior; }

    const ut::Vector &  getPosition() const     { return _position; }
    void                setPosition(const ut::Vector & position);

    const ut::Vector &  getVelocity() const                         { return _velocity; }
    void                setVelocity(const ut::Vector & velocity)    { _velocity = velocity; }

    float   getMass() const     { return _mass; }
    void    setMass(float mass) { _mass = mass; }

    // TODO Bad collision priority get/set name
    int     getPriority() const         { return _collision_priority; }
    void    setPriority(int priority)   { _collision_priority = priority; }

    ut::Rectangle getBoundingBox() const;

    bool    isAwake() const             { return _awake; }
    void    sleep()                     { _awake = false; }
    void    wake()                      { _awake = true; }


#ifndef NDEBUG
    void drawDebug(gfx::DrawContext & debug_context) const;
#endif


    class Comparator
    {
    public:
        bool operator () (const Object & a, const Object & b) const
            { return a.getPriority() < b.getPriority(); }
    };
    // class Comparator

};
// class Object

}
// namespace phy

#endif // _IWBAN_OBJECT_HPP_
