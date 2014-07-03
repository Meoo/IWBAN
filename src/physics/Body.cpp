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
    IWBAN_PRE_PTR(shape);
}

Body::Body(game::SolidEntity * owner, const Shape * shape)
    : _owner(owner), _shape(shape)
{
    IWBAN_PRE_PTR(owner);
    IWBAN_PRE_PTR(shape);
}

Body::~Body()
{
    unlinkChilds();
    setParent(nullptr);
}

void Body::move(const ut::Vector & delta)
{
    // TODO _next_state.immediate_velocity += delta;
    wake();
}

void Body::moveTo(const ut::Vector & position)
{
    // TODO _next_state.immediate_velocity += (position - _next_state.position);
    wake();
}

void Body::unlinkChilds()
{
    for (Body & child : _childs)
        child._parent = nullptr;

    _childs.clear();
}

void Body::setParent(Body * parent)
{
    if (parent == _parent) return;

    if (_parent)
        // Get the iterator from parent list and unlink it
        _parent->_childs.erase(_parent->_childs.iterator_to(*this));

    if (parent)
        parent->_childs.push_back(*this);

    _parent = parent;

    wake();

#ifndef NDEBUG
    // This piece of code is horrible, but works as intended
    const Body * p = parent;
    while (p->getParent())
    {
        IWBAN_ASSERT_MSG(p != this, "Circular parenting detected!");
        p = p->getParent();
    }
#endif
}

/*void Body::setPosition(const ut::Vector & position)
{
    _position = position;
    // TODO just_teleported = true;
    wake();
}*/

/*void Body::setVelocity(const ut::Vector & velocity)
{
    _velocity = velocity;
    wake();
}*/

ut::Rectangle Body::getBoundingBox() const
{
    ut::Rectangle r = getShape()->getBoundingBox();
    r.x += _position.x;
    r.y += _position.y;
    return r;
}

void Body::preUpdate(const sf::Time & delta)
{
    _pressure = ut::Vector();
    _delta = ut::Vector();
    _velocity += _acceleration * delta.asSeconds();
}

void Body::postUpdate(const sf::Time & delta)
{
    _immediate_force = ut::Vector();
}

void Body::preStep(const sf::Time & step_delta)
{
    ut::Vector movement = (_velocity + _immediate_force) * step_delta.asSeconds();
    _position += movement;
    _delta += movement;

    // Update the whole child tree
    moveTree(movement);
}

void Body::postStep(const sf::Time & step_delta)
{
}

void Body::respond(const CollisionResult & result)
{
    _position += result.force;
    _velocity += result.force * 30;
}

void Body::moveTree(const ut::Vector & movement)
{
    for (Body & child : _childs)
    {
        child._position += movement;
        child._delta += movement;

        // Recursion
        child.moveTree(movement);
    }
}

#ifndef NDEBUG
void Body::drawDebug(gfx::DebugContext & debug_context) const
{
    sf::Color col = getColorFromCollisionGroup(getSolidityGroup());

    if (!isAwake())
        col *= sf::Color(128, 128, 128);

    getShape()->drawDebug(debug_context, getPosition(), col);
}
#endif

bool Body::collide(const Body & first, const Body & secnd, CollisionData & data)
{
    if (first.getMass() == 0 && secnd.getMass() == 0)
        return false;

    if (!first.isAwake() && !secnd.isAwake())
        return false;

    // Collision groups
    CollisionGroup first_mask = secnd.getSolidityGroup() & first.getCollisionMask();
    CollisionGroup secnd_mask = first.getSolidityGroup() & secnd.getCollisionMask();

    /*if (first_mask == COL_NONE && secnd_mask == COL_NONE)
        return false;*/

    // Check intersection between objects (before a more precise computation)
    if (!ut::hasIntersection(first.getBoundingBox(), secnd.getBoundingBox()))
        return false;

    data.first       = &first;
    data.second      = &secnd;
    data.first_mask  = first_mask;
    data.second_mask = secnd_mask;

    // Compute the collision data between the two objects
    ut::Vector offset = secnd.getPosition() - first.getPosition();
    return phy::Shape::collide(*first.getShape(), *secnd.getShape(), offset, data);
}

}
// namespace phy
