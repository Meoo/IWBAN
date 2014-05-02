/**
 * @file   Object.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/Behavior.hpp>
#include <physics/Object.hpp>
#include <physics/Shape.hpp>

namespace phy
{

Object::Object(const Shape * shape, Behavior * behavior)
    : _shape(shape), _behavior(behavior),
    _user_data(0), _mass(0),
    _solidity_group(COL_NONE), _collision_mask(COL_NONE)
{
    BOOST_ASSERT(shape);
}

void Object::move(const ut::Vector & delta)
{
    _position += delta;
    wake();
}

void Object::moveTo(const ut::Vector & position)
{
    _position = position;
    wake();
}

void Object::prepare()
{
    _updated = false;

    if (_behavior)
        _behavior->prepare(*this);
}

void Object::step()
{
    if (_updated) return;

    if (hasParent())
        getParent().step();

    if (isAwake())
    {
        if (_behavior)
        {
            _behavior->step(*this);
            stepChilds(*this, _behavior);
        }
        else
            sleep();
    }

    _updated = true;
}

void Object::finish()
{
    updateLastPosition();

    if (_behavior)
        _behavior->finish(*this);
}

void Object::collideWith(Object & other)
{
    // Do not perform narrow phase if both objects do not react to collisions
    if (!getBehavior() && !other.getBehavior())
        return;

    // Collision groups
    CollisionGroup my_mask = other.getSolidityGroup() & getCollisionMask();
    CollisionGroup his_mask  = getSolidityGroup() & other.getCollisionMask();

    if (my_mask == COL_NONE && his_mask == COL_NONE)
        return;

    // Check intersection between objects (before a more precise computation)
    if (!ut::hasIntersection(getBoundingBox(), other.getBoundingBox()))
        return;

    CollisionData data;
    ut::Vector offset = other.getPosition() - getPosition();

    // Compute the collision data between the two objects
    if (phy::collideShapes(getShape(), other.getShape(), offset, data))
    {
        // TODO Response
        data.origin += getPosition();

        if (getBehavior() && (my_mask != COL_NONE))
        {
            getBehavior()->onCollide(*this, other, data);
        }

        if (other.getBehavior() && (his_mask != COL_NONE))
        {
            data.revert();
            other.getBehavior()->onCollide(other, *this, data);
        }
    }
}

bool Object::hasParent() const
{
    // Cast this as ChildHook and check if linked
    return impl::ChildHook::is_linked();
}

Object & Object::getParent()
{
    BOOST_ASSERT(hasParent());
    return *_parent;
}

const Object & Object::getParent() const
{
    BOOST_ASSERT(hasParent());
    return *_parent;
}

void Object::setParent(Object & parent)
{
    BOOST_ASSERT(!hasParent());

    parent._childs.push_back(*this);
    _parent = &parent;

#ifndef NDEBUG
    // This piece of code is horrible, but works as intended
    const Object * p = &parent;
    while (p->hasParent())
    {
        BOOST_ASSERT_MSG(p != this, "Circular parenting detected!");
        p = &(p->getParent());
    }
#endif
}

void Object::unsetParent()
{
    // Cast this as ChildHook and unlink it
    impl::ChildHook::unlink();
    _parent = 0;
}

void Object::setPosition(const ut::Vector & position)
{
    _position = position;
    // TODO just_teleported = true;
    wake();
}

ut::Rectangle Object::getBoundingBox() const
{
    ut::Rectangle r = getShape()->getBoundingBox();
    r.x += _position.x;
    r.y += _position.y;
    return r;
}

void Object::setVelocity(const ut::Vector & velocity)
{
    _velocity = velocity;
    wake();
}

#ifndef NDEBUG
void Object::drawDebug(gfx::DebugContext & debug_context) const
{
    sf::Color col = getColorFromCollisionGroup(getSolidityGroup());

    if (!isAwake())
        col *= sf::Color(128, 128, 128);

    getShape()->drawDebug(debug_context, getPosition(), col);
}
#endif

void Object::stepChilds(Object & parent, Behavior * behavior)
{
    for (Object & child : _childs)
    {
        behavior->stepChild(parent, child);
        child.stepChilds(parent, behavior);
    }
}

}
// namespace phy
