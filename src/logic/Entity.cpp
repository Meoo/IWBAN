/**
 * @file   Entity.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <logic/Entity.hpp>
#include <logic/World.hpp>

namespace logic
{

Entity::Entity()
{
    // TODO Better state machine ?
    getWorld().add(this);
}

Entity::~Entity()
{
    // TODO Exception safe?
    if (_spawned)
        despawn();

    // TODO Better state machine ?
    getWorld().remove(this);
}

// FIXME Debug
void Entity::testRender(gfx::DrawContext & draw)
{
    for (gfx::Drawable * drawable : _drawables)
        draw.draw(*drawable);
}

Variant Entity::getValue(const std::string & key) const
{
    return doGetValue(key);
}

void Entity::setValue(const std::string & key, const Variant & value)
{
    doSetValue(key, value);
}

Variant Entity::doGetValue(const std::string & key) const
{
    Lua & l = getWorld().getLua();

    l.pushObject(getLuaObject());

    // Pop metatable
    lua_getmetatable(l, -1);
    lua_pushnil(l);
    lua_setmetatable(l, -3);

    // Get value
    lua_getfield(l, -2, key.c_str());
    Variant v = l.toVariant(-1);
    lua_pop(l, 1);

    // Push metatable
    lua_setmetatable(l, -2);
    lua_pop(l, 1);

    return v;
}

void Entity::doSetValue(const std::string & key, const Variant & value)
{
    Lua & l = getWorld().getLua();

    l.pushObject(getLuaObject());

    // Pop metatable
    lua_getmetatable(l, -1);
    lua_pushnil(l);
    lua_setmetatable(l, -3);

    // Set value
    l.pushVariant(value);
    lua_setfield(l, -3, key.c_str());

    // Push metatable
    lua_setmetatable(l, -2);
    lua_pop(l, 1);
}

void Entity::addDrawable(gfx::Drawable * drawable)
{
    IWBAN_ASSERT(_spawned);

    _drawables.insert(drawable);
}

void Entity::removeDrawable(gfx::Drawable * drawable)
{
    IWBAN_ASSERT(_spawned);

    _drawables.erase(drawable);
}

void Entity::addBody(phy::Body * body)
{
    IWBAN_ASSERT(_spawned);

    _bodies.insert(body);
    getWorld().getSpace().attach(body);
}

void Entity::removeBody(phy::Body * body)
{
    IWBAN_ASSERT(_spawned);

    getWorld().getSpace().detach(body);
    _bodies.erase(body);
}

void Entity::spawn()
{
    IWBAN_ASSERT(!_spawned);
    _spawned = true;

    doSpawn();
}

void Entity::despawn()
{
    IWBAN_ASSERT(_spawned);

    doDespawn();

    // TODO Better cleanup
    for (phy::Body * body : _bodies)
        getWorld().getSpace().detach(body);

    _spawned = false;
}

void Entity::update()
{
    doUpdate();
}

}
// namespace logic
