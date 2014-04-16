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

Object::Object()
{
}

void Object::move(const ut::Vector & delta)
{
    _position += delta;
}

void Object::teleport(const ut::Vector & position)
{
    _position = position;
    // TODO just_teleported = true;
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

ut::Rectangle Object::getBoundingBox() const
{
    ut::Rectangle r = getShape().getBoundingBox();
    r.x += _position.x;
    r.y += _position.y;
    return r;
}

}
// namespace phy
