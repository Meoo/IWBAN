/**
 * @file   EntityHandle.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <game/EntityHandle.hpp>

#include <game/World.hpp>

namespace game
{

void EntityHandle::reset(Entity * entity)
{
    if (entity)
    {
        _id     = entity->getId();
        _serial = entity->getSerial();
    }
    else
    {
        _id     = Entity::INVALID_ID;
    }
}

Entity * EntityHandle::get()
{
    Entity * e = getWorld().getEntityById(_id);

    if (e->getSerial() == _serial)
        return e;

    return 0;
}

const Entity * EntityHandle::get() const
{
    Entity * e = getWorld().getEntityById(_id);

    if (e->getSerial() == _serial)
        return e;

    return 0;
}

}
// namespace game
