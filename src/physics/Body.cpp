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
    _next_state.immediate_velocity += delta;
    wake();
}

void Body::moveTo(const ut::Vector & position)
{
    _next_state.immediate_velocity += (position - _next_state.position);
    wake();
}

void Body::prepare()
{
    _prev_state = _next_state;

    _next_state.velocity += _acceleration;
    _next_state.immediate_velocity = ut::Vector();

    _prev_state.immediate_velocity += _prev_state.velocity;
}

void Body::step()
{
    _next_state.position += _prev_state.immediate_velocity;

    Body * parent = getParent();
    while (parent)
    {
        _next_state.position += parent->_prev_state.immediate_velocity;
        parent = parent->getParent();
    }
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

ut::Rectangle Body::getBoundingBox() const
{
    ut::Rectangle r = getShape()->getBoundingBox();
    /*r.x += _position.x;
    r.y += _position.y;*/
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

bool Body::collide(const Body & first, const Body & secnd, CollisionData & data)
{
    // Collision groups
    CollisionGroup first_mask = secnd.getSolidityGroup() & first.getCollisionMask();
    CollisionGroup secnd_mask = first.getSolidityGroup() & secnd.getCollisionMask();

    if (first_mask == COL_NONE && secnd_mask == COL_NONE)
        return false;

    // Check intersection between objects (before a more precise computation)
    if (!ut::hasIntersection(first.getBoundingBox(), secnd.getBoundingBox()))
        return false;

    data.first_mask  = first_mask;
    data.second_mask = secnd_mask;

    // Compute the collision data between the two objects
    ut::Vector offset = secnd.getPosition() - first.getPosition();
    return phy::Shape::collide(*first.getShape(), *secnd.getShape(), offset, data);
}

}
// namespace phy
