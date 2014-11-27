/**
 * @file   Entity.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <logic/Entity.hpp>
#include <logic/World.hpp>
#include <physics/Body.hpp>

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
    if (isSpawned())
        IWBAN_LOG_ERROR("Destroying entity '%s' that is still spawned", getName().c_str());

    emergencyCleanup();

    // TODO Better state machine ?
    getWorld().remove(this);
}

Variant Entity::getValue(const std::string & key) const
{
    return doGetValue(key);
}

void Entity::setValue(const std::string & key, const Variant & value)
{
    doSetValue(key, value);
}

void Entity::sendEvent(const std::string & event, const Variant & param)
{
    doEvent(event, param);
}

Variant Entity::doGetValue(const std::string & key) const
{
    // TODO Protect lua calls, or check LuaObject
    IWBAN_PRE(getLuaObject().isValid());

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
    IWBAN_PRE(getLuaObject().isValid());

    Lua & l = getLua();

    l.pushObject(getLuaObject());
    lua_pushstring(l, key.c_str());
    l.pushVariant(value);
    lua_rawset(l, -3);
    lua_pop(l, 1);
}

void Entity::doEvent(const std::string & event, const Variant & param)
{
    // TODO Protect lua calls, or check LuaObject
    IWBAN_PRE(getLuaObject().isValid());

    Lua & l = getLua();

    l.pushObject(getLuaObject());
    lua_pushstring(l, event.c_str());
    lua_rawget(l, -2);

    if (lua_type(l, -1) == LUA_TFUNCTION)
    {
        lua_pushvalue(l, -2); // self
        lua_remove(l, -3);
        l.pushVariant(param); // param
        l.pcall(2, 0);        // Call ENT:event(param)
    }
    else
    {
        lua_pop(l, 2);
        IWBAN_LOG_WARNING("Unhandled event '%s' on Entity '%s'", event.c_str(), getName().c_str());
    }
}

World & Entity::getWorld()
{
    return logic::getWorld();
}

gfx::Scene & Entity::getScene()
{
    return logic::getWorld().getScene();
}

phy::Space & Entity::getSpace()
{
    return logic::getWorld().getSpace();
}

Lua & Entity::getLua() const
{
    return logic::getWorld().getLua();
}

void Entity::scheduleNextUpdate(sys::FTimeOffset delay)
{
    scheduleNextUpdateAt(getWorld().getClock() + delay);
}

void Entity::addDrawable(gfx::Drawable * drawable)
{
    IWBAN_PRE(_spawned);

    getScene().add(drawable);
    _drawables.insert(drawable);
}

void Entity::removeDrawable(gfx::Drawable * drawable)
{
    IWBAN_PRE(_spawned);

    getScene().remove(drawable);
    _drawables.erase(drawable);
}

void Entity::addBody(phy::Body * body)
{
    IWBAN_PRE(_spawned);

    getSpace().add(body, this);
    _bodies.insert(body);
}

void Entity::removeBody(phy::Body * body)
{
    IWBAN_PRE(_spawned);

    getSpace().remove(body);
    _bodies.erase(body);
}

void Entity::spawn()
{
    IWBAN_PRE(!_spawned);
    _spawned = true;

    doSpawn();
}

void Entity::despawn()
{
    IWBAN_PRE(_spawned);

    doDespawn();

    emergencyCleanup();

    _spawned = false;
}

void Entity::update()
{
    doUpdate();
}

void Entity::emergencyCleanup()
{
    if (!_bodies.empty())
        IWBAN_LOG_ERROR("Entity '%s' has bodies left", getName().c_str());

    if (!_drawables.empty())
        IWBAN_LOG_ERROR("Entity '%s' has drawables left", getName().c_str());

    if (!_shadows.empty())
        IWBAN_LOG_ERROR("Entity '%s' has shadows left", getName().c_str());

    if (!_lights.empty())
        IWBAN_LOG_ERROR("Entity '%s' has lights left", getName().c_str());
}

}
// namespace logic
