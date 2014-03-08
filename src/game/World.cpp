/**
 * @file   World.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <game/World.hpp>

namespace game
{

Entity * World::getEntityById(Entity::Id id)
{
    BOOST_ASSERT_MSG(id >= 0 && id < IWBAN_MAX_ENTITIES
                  && id != Entity::INVALID_ID, "Invalid value for id param");

    if (id == Entity::INVALID_ID)
        return 0;

    PoolNode * n = _entities.at(id);
    if (n->getValue().in_use)
        return n->getValue().entity;

    return 0;
}

void World::update()
{
    // TODO We do not do it after update so we can still render entities that just died (MAYBE?)
    cleanDeadEntities();

    updateEntities();

    spawnNewEntities();

    // Collision detection : Broad phase


    // Collision detection : Narrow phase


    // Collision response

}

void World::render(gfx::Renderer & renderer) const
{
    renderEntities(renderer);
}

// ---- ---- ---- ----

World::World()
{
    _next_serial = 0;
}

void World::updateEntities()
{
    PoolNode * node = _valid_ents_list.getFirst();
    const void * end = _valid_ents_list.getSentry();

    while (node != end)
    {
        // Call update function
        node->getValue().entity->update();

        node = node->getNext();
    }
}

void World::renderEntities(gfx::Renderer & renderer) const
{
    const PoolNode * node = _valid_ents_list.getFirst();
    const void * end = _valid_ents_list.getSentry();

    while (node != end)
    {
        // TODO Call render function


        node = node->getNext();
    }
}

void World::spawnNewEntities()
{
    PoolNode * node = _new_ents_list.getFirst();
    const void * end = _new_ents_list.getSentry();

    // We can create entities within spawn function
    while (node != end)
    {
        // Call spawn function
        node->getValue().entity->spawn();

        node = node->getNext();
    }

    // Move all spawned entities to the valid list
    _valid_ents_list.insertTail(_new_ents_list.getFirst(),
                                _new_ents_list.getLast());
}

void World::cleanDeadEntities()
{
    PoolNode * node = _valid_ents_list.getFirst();
    const void * end = _valid_ents_list.getSentry();

    while (node != end)
    {
        EntityDesc & desc = node->getValue();

        if (!desc.entity->isAlive())
        {
            // TODO Call despawn function or something, and destructor


            // Free the descriptor
            desc.entity = 0;
            desc.in_use = false;

            PoolNode * dead_node = node;
            node = node->getNext();
            _entities.free(dead_node);
        }
        else
            node = node->getNext();
    }
}

void World::registerEntity(Entity * entity)
{
    BOOST_ASSERT_MSG(entity, "Entity cannot be null");

    PoolNode *      node    = _entities.alloc();

    Entity::Id      id      = _entities.getIdFromNode(node);
    Entity::Serial  serial  = _next_serial++;
    EntityDesc &    desc    = node->getValue();

    // Setup descriptor
    desc.entity = entity;
    desc.in_use = true;

    // Move the descriptor to the new list
    _new_ents_list.insertTail(node);

    // Assign new values to entity
    entity->setId(id);
    entity->setSerial(serial);
}

/*
void World::unregisterEntity(Entity * entity)
{
    BOOST_ASSERT_MSG(entity, "Entity cannot be null");

    Entity::Id      id      = entity->getId();
    PoolNode *      node    = _entities.at(id);
    EntityDesc &    desc    = node->value;

    BOOST_ASSERT_MSG(id >= 0 && id < IWBAN_MAX_ENTITIES
                  && id != Entity::INVALID_ID, "Entity id is invalid");
    BOOST_ASSERT_MSG(desc.entity == entity, "Entity / descriptor mismatch");

    // Free the descriptor
    desc.entity = 0;
    desc.in_use = false;

    _entities.free(node);
}*/

}
// namespace game
