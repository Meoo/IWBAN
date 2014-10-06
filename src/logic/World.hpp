/**
 * @file   World.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_WORLD_HPP_
#define _IWBAN_WORLD_HPP_

#include <Global.hpp>

#include <logic/Lua.hpp>
#include <physics/Space.hpp>

#include <set>

namespace logic
{

class World
{
private:
    phy::Space          _space;
    Lua                 _lua;

    std::set<Entity *>  _new_entities;
    std::set<Entity *>  _entities;


public:
    Lua &   getLua() { return _lua; }

    // Get all entities
    void    getEntities();

    void    update();

    // FIXME Debug
    void    testRender(gfx::DrawContext & draw);

    // TODO Private for Entity ? Debug draw ?
    phy::Space & getSpace() { return _space; }


private:
    friend class Entity;

    // Called by Entity constructor / destructor
    void    add(Entity * entity);
    void    remove(Entity * entity) noexcept;


};
// class World

// ---- ---- ---- ----

inline
World & getWorld()
{
    static World s_world;
    return s_world;
}

}
// namespace logic

#endif // _IWBAN_WORLD_HPP_
