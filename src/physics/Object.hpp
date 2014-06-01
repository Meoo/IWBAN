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
#  include <graphics/contexts/debug/DebugContext.hpp>
#endif

namespace phy
{

class Behavior;
class Object;
class Shape;

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

// Boost's hooks must use public inheritance
class Object : public impl::ObjectHook,
               public impl::ChildHook
{
public:
    typedef impl::ObjectList    List;
    typedef impl::ChildList     ChildList;


private:
    // Data members
    const Shape *   _shape;

    Object *        _parent;
    ChildList       _childs;

    Behavior *      _behavior;
    void *          _user_data;

    ut::Vector      _position;
    ut::Vector      _last_position;

    ut::Vector      _velocity;

    float           _mass;

    CollisionGroup  _solidity_group;
    CollisionGroup  _collision_mask;

    bool            _updated;
    bool            _awake;

    // TODO Object additional states
    //bool just_teleported;
    //bool smooth;


public:
    // Constructor
    explicit Object(const Shape * shape, Behavior * behavior = 0);

    // Functions
    void    move(const ut::Vector & delta);
    void    moveTo(const ut::Vector & position);

    void    prepare();
    void    step();
    void    finish();

    void    collideWith(Object & other);

    bool    hasParent() const;
    Object & getParent();
    const Object & getParent() const;
    void    setParent(Object & parent);
    void    unsetParent();
    // TODO getParent()? not required?

    // Getters / setters
    const Shape * getShape() const              { return _shape; }
    void          setShape(const Shape * shape) { BOOST_ASSERT(shape); _shape = shape; }

    Behavior * getBehavior() const              { return _behavior; }
    void       setBehavior(Behavior * behavior) { _behavior = behavior; }

    void *      getUserData() const             { return _user_data; }
    void        setUserData(void * user_data)   { _user_data = user_data; }

    const ut::Vector &  getPosition() const     { return _position; }
    void                setPosition(const ut::Vector & position);

    const ut::Vector &  getLastPosition() const { return _last_position; }
    void                updateLastPosition()    { _last_position = _position; }

    const ut::Vector &  getVelocity() const     { return _velocity; }
    void                setVelocity(const ut::Vector & velocity);

    float   getMass() const     { return _mass; }
    void    setMass(float mass) { _mass = mass; }

    CollisionGroup getSolidityGroup() const     { return _solidity_group; }
    void setSolidityGroup(CollisionGroup group) { _solidity_group = group; }

    CollisionGroup getCollisionMask() const     { return _collision_mask; }
    void setCollisionMask(CollisionGroup mask)  { _collision_mask = mask; }

    ut::Rectangle getBoundingBox() const;

    bool    isAwake() const     { return _awake; }
    void    sleep()             { _awake = false; }
    void    wake()              { _awake = true; }


#ifndef NDEBUG
    void drawDebug(gfx::DebugContext & debug_context) const;
#endif


protected:
    void    stepChilds(Object & parent, Behavior * behavior);

};
// class Object

}
// namespace phy

#endif // _IWBAN_OBJECT_HPP_
