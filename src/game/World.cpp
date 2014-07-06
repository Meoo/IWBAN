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
        return nullptr;

    for (unsigned i = 0; i < IWBAN_MAX_ENTITIES; ++i)
        _free_slots.push_back(IWBAN_MAX_ENTITIES - i - 1);

    return nullptr;
}

void World::update()
{
    // Tick clock
    ++_clock;

    // TODO We do not do it after update so we can still render entities that just died (MAYBE?)
    cleanDeadEntities();

    updateEntities();

    spawnNewEntities();

    // Update the physical space
    _space.update(IWBAN_UPDATE_TIME, 4);

    pumpEvents();

    // Sort the drawables
    ut::doubleBubbleSort(_drawables.begin(), _drawables.end(),
        [](const gfx::Drawable * a, const gfx::Drawable * b)
        {
            return a->getDepth() < b->getDepth();
        });

}

void World::render(gfx::Renderer & renderer) const
{
    gfx::DrawContext & draw = renderer.openDrawContext();

    // Draw every drawable in the DrawContext
    for (const gfx::Drawable * drawable : _drawables)
        draw.draw(*drawable);

    draw.close();
}

void World::queueEvent(Entity * source, Entity * target, Event && event)
{
    queueDelayedEvent(source, target, std::move(event), 0);
}

void World::queueEvent(Entity * source, const std::string & target_name, Event && event)
{
    queueDelayedEvent(source, target_name, std::move(event), 0);
}

void World::queueDelayedEvent(Entity * source, Entity * target,
                              Event && event, sys::FTimeOffset delay)
{
    // TODO Can be a warning
    IWBAN_PRE_PTR(source);
    IWBAN_PRE_PTR(target);

    sys::FTime time = _clock + delay;

    EventEntry entry(std::move(event));
    entry.source.reset(source);
    entry.target_type = EventEntry::TGT_HANDLE;
    entry.target_handle.reset(target);
    entry.time = time;

    // Insert before the first event who have a higher time
    // TODO Start from the end of the list to improve performances a little
    _event_list.insert(std::find_if(_event_list.begin(), _event_list.end(),
            [time](const EventEntry & e) { return e.time > time; }
        ), std::move(entry));
}

void World::queueDelayedEvent(Entity * source, const std::string & target_name,
                              Event && event, sys::FTimeOffset delay)
{
    // TODO Can be a warning
    IWBAN_PRE_PTR(source);
    IWBAN_PRE(!target_name.empty());

    sys::FTime time = _clock + delay;

    EventEntry entry(std::move(event));
    entry.source.reset(source);
    entry.target_type = EventEntry::TGT_NAME;
    entry.target_name = target_name;
    entry.time = time;

    // Insert before the first event who have a higher time
    // TODO Start from the end of the list to improve performances a little
    _event_list.insert(std::find_if(_event_list.begin(), _event_list.end(),
            [time](const EventEntry & e) { return e.time > time; }
        ), std::move(entry));
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
    while (_event_list.front().time <= _clock)
    {
        EventEntry e = std::move(_event_list.front());
        _event_list.pop_front();

        // Process event
        Entity * source = e.source.get();

        if (e.target_type == EventEntry::TGT_HANDLE)
        {
            Entity * target = e.target_handle.get();

            if (!target)
            {
                IWBAN_LOG_DEBUG("Processed event with lost target\n");
                return;
            }

            target->onEvent(source, e.event);
        }
        else // target_type == TGT_NAME
        {
            auto range = _entity_index.equal_range(e.target_name);

            if (range.first == range.second)
            {
                IWBAN_LOG_DEBUG("Processed event with no matching target\n");
                return;
            }

            for (auto it = range.first; it != range.second; ++it)
            {
                it->second->onEvent(source, e.event);
            }
        }
    }
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

        // TODO An exception may be appropriate here
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
