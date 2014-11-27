/**
 * @file   World.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_WORLD_HPP_
#define _IWBAN_WORLD_HPP_

#include <Global.hpp>

#include <logic/Lua.hpp>
#include <graphics/Scene.hpp>
#include <physics/Space.hpp>
#include <system/Time.hpp>

#include <memory>
#include <set>

namespace logic
{

class World
{
private:
    gfx::Scene          _scene;
    phy::Space          _space;
    Lua                 _lua;

    sys::FTime          _clock          = 0;

    std::set<Entity *>  _new_entities;
    std::set<Entity *>  _entities;


public:
    // Subsystems
    // TODO Private for Entity ? Debug draw ?
    Lua &           getLua()            { return _lua; }
    gfx::Scene &    getScene()          { return _scene; }
    phy::Space &    getSpace()          { return _space; }

    // Getters / Setters
    sys::FTime      getClock() const    { return _clock; }

    // Entity management functions
    std::unique_ptr<Entity> createEntity(const std::string & entity_class);
    Entity *                ownEntity(std::unique_ptr<Entity> entity);
    Entity *                createAndOwnEntity(const std::string & entity_class);

    // Functions
    void    update();


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
