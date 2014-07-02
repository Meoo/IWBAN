/**
 * @file   Body.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BODY_HPP_
#define _IWBAN_BODY_HPP_

#include <Global.hpp>

#include <game/entities/SolidEntity.hpp>

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


private:
    // Data members
    game::SolidEntity * _owner      = nullptr;

    const Shape *   _shape;

    Body *          _parent         = nullptr;
    ChildList       _childs;

    ut::Vector      _position;
    ut::Vector      _velocity;
    ut::Vector      _acceleration;
    ut::Vector      _immediate_force;

    ut::Vector      _delta;
    ut::Vector      _pressure;

    // Mass <= 0 means infinite (cannot be moved by other bodies)
    float           _mass           = 0;
    float           _damping        = 0;
    float           _air_resist     = 0;

    CollisionGroup  _solidity_group = COL_NONE;
    CollisionGroup  _collision_mask = COL_NONE;

    bool            _awake          = true;

    // TODO Object additional states
    //bool just_teleported;
    //bool smooth;


public:
    // Constructor
    explicit        Body(const Shape * shape);
                    Body(game::SolidEntity * owner, const Shape * shape);

    // Destructor
                    ~Body();

    // Functions
    void            move(const ut::Vector & delta);
    void            moveTo(const ut::Vector & position);

    void            unlinkChilds();

    // TODO Add wake everytime any property is modified
    // Getters / setters
    const ut::Vector & getPosition() const          { return _position; }
    const ut::Vector & getVelocity() const          { return _velocity; }

    const game::SolidEntity *   getOwner() const    { return _owner; }
    game::SolidEntity *         getOwner()          { return _owner; }

    const Shape *   getShape() const                { return _shape; }
    void            setShape(const Shape * shape)   { IWBAN_PRE_PTR(shape); _shape = shape; }

    Body *          getParent()                     { return _parent; }
    const Body *    getParent() const               { return _parent; }

    void            setParent(Body * parent);

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

    void            preUpdate(const sf::Time & delta);
    void            postUpdate(const sf::Time & delta);

    void            preStep(const sf::Time & step_delta);
    void            postStep(const sf::Time & step_delta);

    void            moveTree(const ut::Vector & movement);


#ifndef NDEBUG
    void            drawDebug(gfx::DebugContext & debug_context) const;
#endif

    // Static functions
    /**
     * Compute the collision data between two bodies.
     *
     * You should call #canCollide on the two bodies first.
     */
    static bool     collide(const Body & first, const Body & second, CollisionData & data);

};
// class Object

}
// namespace phy

#endif // _IWBAN_BODY_HPP_
