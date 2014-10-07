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
    _lua_object = getLua().createObject(LUA_IWBAN_ENTITIES);
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
    // TODO Protect lua calls, or check LuaObject
    IWBAN_ASSERT(getLuaObject().isValid());

    Lua & l = getLua();

    l.pushObject(getLuaObject());
    lua_pushstring(l, key.c_str());
    lua_rawget(l, -2);
    Variant v = l.toVariant(-1);
    lua_pop(l, 2);

    return v;
}

void Entity::doSetValue(const std::string & key, const Variant & value)
{
    // TODO Protect lua calls, or check LuaObject
    IWBAN_ASSERT(getLuaObject().isValid());

    Lua & l = getLua();

    l.pushObject(getLuaObject());
    lua_pushstring(l, key.c_str());
    l.pushVariant(value);
    lua_rawset(l, -3);
    lua_pop(l, 1);
}

World & Entity::getWorld()
{
    return logic::getWorld();
}

phy::Space & Entity::getSpace()
{
    return logic::getWorld().getSpace();
}

Lua & Entity::getLua() const
{
    return logic::getWorld().getLua();
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
