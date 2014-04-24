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
    : _shape(shape), _behavior(behavior), _mass(0)
{
    BOOST_ASSERT(shape);
}

void Object::move(const ut::Vector & delta)
{
    _position += delta;
    wake();
}

void Object::step()
{
    if (_behavior)
    {
        _behavior->step(*this);

        // TODO Recursive?
        for (Object & child : _childs)
            _behavior->stepChild(*this, child);
    }
    else
        sleep();
}

void Object::collideWith(Object & other)
{
    // Do not perform narrow phase if both objects do not react to collisions
    if (!getBehavior() && !other.getBehavior())
        return;

    // TODO Collision groups
    /*if (!(_solidity & other._collides_with)
     || !(other._solidity & _collides_with))
        return;*/

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

        if (getBehavior())
            getBehavior()->onCollide(*this, other, data);

        if (other.getBehavior())
        {
            data.revert();
            other.getBehavior()->onCollide(other, *this, data);
        }
    }
}

void Object::setParent(Object & parent)
{
    parent._childs.push_back(*this);
}

void Object::unsetParent()
{
    // Cast this as ChildHook and unlink it
    impl::ChildHook::unlink();
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

#ifndef NDEBUG
void Object::drawDebug(gfx::DrawContext & debug_context) const
{
    sf::Color col = getColorFromCollisionGroup(_solidity);

    if (!isAwake())
        col *= sf::Color(128, 128, 128);

    getShape()->drawDebug(debug_context, getPosition(), col);
}
#endif

}
// namespace phy
