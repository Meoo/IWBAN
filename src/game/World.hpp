/**
 * @file   World.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_WORLD_HPP_
#define _IWBAN_WORLD_HPP_

#include <Global.hpp>

#include <game/Entity.hpp>
#include <game/Event.hpp>

#include <graphics/Drawable.hpp>
#include <graphics/Renderer.hpp> // TODO Fwd decl?

#include <physics/Space.hpp>

#include <system/Time.hpp>

#include <array>
#include <list>
#include <unordered_map>
#include <set>

namespace game
{

class World
{
public:
    class EventEntry
    {
    public:
        EntityHandle    source;

        enum
        {
            TGT_NAME,
            TGT_HANDLE

        } const         target_type;

        std::string     target_name;
        EntityHandle    target_handle;

        Event           event;

        sys::FTime      time;

        // Constructors
        EventEntry(Event && event, Entity * source,
                   const std::string & target, sys::FTime time)
            : source(source)
            , target_type(TGT_NAME), target_name(target)
            , event(std::move(event)), time(time) {}

        EventEntry(Event && event, Entity * source,
                   Entity * target, sys::FTime time)
            : source(source)
            , target_type(TGT_HANDLE), target_handle(target)
            , event(std::move(event)), time(time) {}

    };
    // class EventEntry

    typedef std::list<gfx::Drawable *> DrawableList;

    typedef std::list<EventEntry> EventList;

    typedef std::array<Entity *, IWBAN_MAX_ENTITIES> EntityTable;
    typedef std::unordered_multimap<std::string, Entity *> EntityIndex;


private:
    // Data members
    EntityTable         _entities;
    EntityIndex         _entity_index;

    std::vector<unsigned> _free_slots;
    Entity::Serial      _next_serial    = 1;

    sys::FTime          _clock          = 0;

    EventList           _event_list;

    DrawableList        _drawables;
    phy::Space          _space;


public:
                World() = default;

    Entity *    getEntityById(Entity::Id id);

    std::vector<Entity *> getEntitiesByName(const std::string & name);

    void        update();

    void        render(gfx::Renderer & renderer) const;

    void        queueEvent(Entity * source, Entity * target, Event && event);
    void        queueEvent(Entity * source, const std::string & target_name, Event && event);

    void        queueDelayedEvent(Entity * source, Entity * target,
                                  Event && event, sys::FTimeOffset delay);
    void        queueDelayedEvent(Entity * source, const std::string & target_name,
                                  Event && event, sys::FTimeOffset delay);

    sys::FTime  getClock() const { return _clock; }


private:
    // Internal functions
    void    updateEntities();
    void    spawnNewEntities();
    void    cleanDeadEntities();

    void    pumpEvents();

    void    spawnEntity(Entity * entity);
    void    despawnEntity(Entity * entity);

};
// class World

}
// namespace game

#endif // _IWBAN_WORLD_HPP_
