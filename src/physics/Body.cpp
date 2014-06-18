/**
 * @file   Body.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/Body.hpp>
#include <physics/Shape.hpp>

namespace phy
{

Body::Body(const Shape * shape)
    : _shape(shape)
{
    BOOST_ASSERT(shape);
}

void Body::move(const ut::Vector & delta)
{
    _position += delta;
    wake();
}

void Body::moveTo(const ut::Vector & position)
{
    _position = position;
    wake();
}

/*void Body::prepare()
{
    _updated = false;

    if (_behavior)
        _behavior->prepare(*this);
}*/

void Body::step()
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

/*void Body::finish()
{
    updateLastPosition();

    if (_behavior)
        _behavior->finish(*this);
}*/

/*void Body::collideWith(Body & other)
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
}*/

/*bool Body::hasParent() const
{
    // Cast this as ChildHook and check if linked
    return impl::ChildHook::is_linked();
}

Body & Body::getParent()
{
    BOOST_ASSERT(hasParent());
    return *_parent;
}

const Body & Body::getParent() const
{
    BOOST_ASSERT(hasParent());
    return *_parent;
}

void Body::setParent(Body & parent)
{
    BOOST_ASSERT(!hasParent());

    parent._childs.push_back(*this);
    _parent = &parent;

#ifndef NDEBUG
    // This piece of code is horrible, but works as intended
    const Body * p = &parent;
    while (p->hasParent())
    {
        BOOST_ASSERT_MSG(p != this, "Circular parenting detected!");
        p = &(p->getParent());
    }
#endif
}

void Body::unsetParent()
{
    // Cast this as ChildHook and unlink it
    impl::ChildHook::unlink();
    _parent = 0;
}*/

/*void Body::setPosition(const ut::Vector & position)
{
    _position = position;
    // TODO just_teleported = true;
    wake();
}*/

ut::Rectangle Body::getBoundingBox() const
{
    ut::Rectangle r = getShape()->getBoundingBox();
    r.x += _position.x;
    r.y += _position.y;
    return r;
}

/*void Body::setVelocity(const ut::Vector & velocity)
{
    _velocity = velocity;
    wake();
}*/

#ifndef NDEBUG
void Body::drawDebug(gfx::DebugContext & debug_context) const
{
    sf::Color col = getColorFromCollisionGroup(getSolidityGroup());

    if (!isAwake())
        col *= sf::Color(128, 128, 128);

    getShape()->drawDebug(debug_context, getPosition(), col);
}
#endif

}
// namespace phy
