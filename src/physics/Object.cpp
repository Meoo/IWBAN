/**
 * @file   Object.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/Behavior.hpp>
#include <physics/Object.hpp>

namespace phy
{

void Object::move(const ut::Vector & delta)
{
    _position += delta;
}

void Object::teleport(const ut::Vector & position)
{
    _position = position;
    // TODO just_teleported = true;
}

void Object::prepare()
{
    _last_position = _position;
}

void Object::update()
{
    if (_behavior)
    {
        _behavior->update(*this);

        for (Object & child : _childs)
            _behavior->updateChild(*this, child);
    }
}

void Object::setParent(Object & parent)
{
    parent._childs.push_back(*this);
}

void Object::unsetParent()
{
    // TODO Unlink self from parent's child list
    // Something really wrong, like
    // ChildHook::unlink();
    // Where ChildHook should be a huge typedef for boost intrusive child list hook
}

}
// namespace phy
