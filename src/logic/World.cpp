/**
 * @file   World.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <logic/Entity.hpp>
#include <logic/World.hpp>

namespace logic
{

void World::update()
{
    for (Entity * entity : _new_entities)
    {
        entity->spawn();
        _entities.insert(entity);
    }
    _new_entities.clear();

    _space.update();

    for (Entity * ent : _entities)
    {
        // TODO Check next update value, or sort by next update
        ent->update();
    }
}

void World::add(Entity * entity)
{
    _new_entities.insert(entity);
}

void World::remove(Entity * entity) noexcept
{
    _new_entities.erase(entity);
    _entities.erase(entity);
}

}
// namespace logic
