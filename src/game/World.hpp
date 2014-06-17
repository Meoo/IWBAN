/**
 * @file   World.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_WORLD_HPP_
#define _IWBAN_WORLD_HPP_

#include <Global.hpp>

#include <game/Entity.hpp>

#include <graphics/Drawable.hpp>
#include <graphics/Renderer.hpp> // TODO Fwd decl?

#include <system/Time.hpp>

#include <array>
#include <list>
#include <set>

namespace game
{

class World
{
private:
    // Data members
    std::array<Entity *, IWBAN_MAX_ENTITIES> _entities;

    std::vector<unsigned> _free_slots;

    Entity::Serial      _next_serial    = 1;

    sys::FTime          _clock;

    std::list<Event>    _event_list;

    gfx::Drawable::List _drawables;


public:
                World() = default;

    Entity *    getEntityById(Entity::Id id);

    void        update();

    void        render(gfx::Renderer & renderer) const;

    void        queueEvent(Entity * source, Entity * target, Event event);

    void        queueDelayedEvent(Entity * source, Entity * target, Event event, sys::FTimeOffset delay);

    sys::FTime  getClock() const { return _clock; }


private:
    // Internal functions
    void    updateEntities();

    void    spawnNewEntities();

    void    cleanDeadEntities();

    void    pumpEvents();


    void    spawnEntity(Entity * entity);

    /*TODO void    unregisterEntity(Entity * entity);*/

};
// class World

}
// namespace game

#endif // _IWBAN_WORLD_HPP_
