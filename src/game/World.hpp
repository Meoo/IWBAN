/**
 * @file   World.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_WORLD_HPP_
#define _IWBAN_WORLD_HPP_

#include <Global.hpp>

#include <game/Entity.hpp>

#include <graphics/Renderer.hpp> // TODO Fwd decl?
#include <graphics/Drawable.hpp>

#include <utils/IndexedPool.hpp>

namespace game
{

class World
{
public:
    // Singleton accessor
    friend World & game::getWorld();


private:
    // Entity descriptors
    class EntityDesc
    {
    public:
        Entity *    entity;
        bool        in_use;

        EntityDesc() : entity(0), in_use(false) {}

    };

    typedef ut::IndexedPool<EntityDesc, IWBAN_ENTS_BLK_SIZE,
                                        IWBAN_ENTS_BLK_COUNT> EntityDescPool;

    typedef EntityDescPool::Node    PoolNode;
    typedef EntityDescPool::List    EntityList;

    // Data members
    EntityDescPool  _entities;

    EntityList      _valid_ents_list;
    EntityList      _new_ents_list;

    Entity::Serial  _next_serial;


public:
    Entity *    getEntityById(Entity::Id id);

    void        update();

    void        render(gfx::Renderer & renderer) const;

    // TODO broadcast event


private:
    // Private constructor for singleton
            World();

    void    updateEntities();

    void    renderEntities(gfx::Renderer & renderer) const;

    void    spawnNewEntities();

    void    cleanDeadEntities();

    void    registerEntity(Entity * entity);

    /*TODO void    unregisterEntity(Entity * entity);*/

};
// class World

// ---- ---- ---- ----

inline World & getWorld()
{
    static World s_world;
    return s_world;
}

}
// namespace game

#endif // _IWBAN_WORLD_HPP_
