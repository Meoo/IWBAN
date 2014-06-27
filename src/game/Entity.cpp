/**
 * @file   Entity.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <game/Entity.hpp>

#include <game/World.hpp>

namespace game
{

void EntityHandle::reset(Entity * entity)
{
    if (entity)
    {
        IWBAN_PRE(entity->isValid());

        _world  = &(entity->getWorld());
        _id     = entity->getId();
        _serial = entity->getSerial();
    }
    else
    {
        _world  = nullptr;
        _id     = Entity::INVALID_ID;
    }
}

Entity * EntityHandle::get()
{
    if (!_world)
        return nullptr;

    Entity * e = _world->getEntityById(_id);

    if (e->getSerial() == _serial && e->isValid())
        return e;

    return nullptr;
}

const Entity * EntityHandle::get() const
{
    if (!_world)
        return nullptr;

    Entity * e = _world->getEntityById(_id);

    if (e->getSerial() == _serial && e->isValid())
        return e;

    return nullptr;
}

}
// namespace game
