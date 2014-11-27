/**
 * @file   World.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <logic/Entity.hpp>
#include <logic/World.hpp>

namespace logic
{

std::unique_ptr<Entity> World::createEntity(const std::string & entity_class)
{
    // TODO World::createEntity
    return std::unique_ptr<Entity>(nullptr);
}

Entity * World::ownEntity(std::unique_ptr<Entity> entity)
{
    // TODO World::ownEntity
    return nullptr;
}

Entity * World::createAndOwnEntity(const std::string & entity_class)
{
    return ownEntity(createEntity(entity_class));
}

void World::update()
{
    ++_clock;

    for (Entity * entity : _new_entities)
    {
        entity->spawn();
        _entities.insert(entity);
    }
    _new_entities.clear();

    _space.update();

    for (Entity * ent : _entities)
        if (ent->getNextUpdateDate() == getClock())
            ent->update();
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
