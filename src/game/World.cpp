/**
 * @file   World.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <game/World.hpp>

#include <utils/Sort.hpp>

#include <algorithm> // find_if

namespace game
{

Entity * World::getEntityById(Entity::Id id)
{
    BOOST_ASSERT_MSG(id >= 0 && id < IWBAN_MAX_ENTITIES
                  && id != Entity::INVALID_ID, "Invalid value for id param");

    if (id == Entity::INVALID_ID)
        return 0;

    for (unsigned i = 0; i < IWBAN_MAX_ENTITIES; ++i)
        _free_slots.push_back(IWBAN_MAX_ENTITIES - i - 1);

    return 0;
}

void World::update()
{
    // Tick clock
    ++_clock;

    // TODO We do not do it after update so we can still render entities that just died (MAYBE?)
    cleanDeadEntities();

    // Update entities
    updateEntities();

    spawnNewEntities();

    // Update the physical space
    _space.refresh();

    // Collision detection : Broad phase
    _space.computePairs([this](const phy::Body & first_body,
                               const phy::Body & second_body)
    {
        // Collision detection : Narrow phase
        phy::CollisionData data;
        if(phy::Body::collide(first_body, second_body, data))
        {
            // Collision response
        }
    });

    // Pump events
    pumpEvents();

}

void World::render(gfx::Renderer & renderer) const
{
    // Sort the drawables
    ut::doubleBubbleSort(_drawables.begin(), _drawables.end(),
                         gfx::Drawable::Comparator());

    gfx::DrawContext & draw = renderer.openDrawContext();

    // Draw every drawable in the DrawContext
    for (const gfx::Drawable & drawable : _drawables)
        draw.draw(drawable);

    draw.close();
}

void World::queueEvent(Entity * source, Entity * target, Event && event)
{
    queueDelayedEvent(source, target, std::move(event), 0);
}

void World::queueDelayedEvent(Entity * source, Entity * target,
                              Event && event, sys::FTimeOffset delay)
{
    //Event event_table(event);
    //sys::FTime time = _clock + delay;

    // Insert before the first event who have a higher time
    // TODO Start from the end of the list to improve performances a little
    /*_event_list.insert(std::find_if(_event_list.begin(), _event_list.end(),
            [time](auto it) { return it->time > time; }
        ), std::move(event_table));*/
}

// ---- ---- ---- ----

void World::updateEntities()
{
    for (Entity * entity : _entities)
    {
        if (entity && entity->isAlive() && entity->_next_update == _clock)
            entity->onUpdate();
    }
}

void World::spawnNewEntities()
{

}

void World::cleanDeadEntities()
{
    // despawnEntity(e)
}

void World::pumpEvents()
{
    /*Event e;
    while ((e = _event_list.front()).time <= _clock)
    {
         _event_list.pop_front();

         // TODO Process event
         Entity * source;
         Entity * target;

    }*/
}

void World::spawnEntity(Entity * entity)
{
    BOOST_ASSERT_MSG(entity, "Entity cannot be null");
    BOOST_ASSERT_MSG(!_free_slots.empty(), "No free entity slot");

    // Add to Entity table
    Entity::Id id = _free_slots.back();
    _free_slots.pop_back();
    _entities[id] = entity;

    if (!entity->getName().empty())
    {
        // Add to Entity index
        _entity_index.insert({entity->getName(), entity});
    }

    entity->spawn(this, id, _next_serial++);
}

void World::despawnEntity(Entity * entity)
{
    BOOST_ASSERT_MSG(entity, "Entity cannot be null");
    BOOST_ASSERT_MSG(_entities[entity->getId()] == entity, "Entity does not belong to this World");

    // Remove from Entity table
    _free_slots.push_back(entity->getId());
    _entities[entity->getId()] = nullptr;

    // Remove from Entity index
    if (!entity->getName().empty())
    {
        // Make sure that one and only one is removed
        bool removed = false;

        auto range = _entity_index.equal_range(entity->getName());
        for (auto it = range.first; it != range.second; ++it)
        {
            if (it->second == entity)
            {
                _entity_index.erase(it);
                removed = true;
                break;
            }
        }

        if (!removed)
            IWBAN_LOG_ERROR("Cannot remove Entity %u '%s' from index!\n",
                            entity->getId(), entity->getName().c_str());
    }

    entity->despawn();

    // TODO Delete entity ?
    delete entity;
}

}
// namespace game
