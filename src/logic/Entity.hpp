/**
 * @file   Entity.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ENTITY_HPP_
#define _IWBAN_ENTITY_HPP_

#include <Global.hpp>

#include <graphics/Drawable.hpp>
#include <graphics/Light.hpp>
#include <graphics/ShadowVolume.hpp>
#include <logic/Lua.hpp>
#include <logic/Variant.hpp>
#include <system/Time.hpp>

#include <boost/noncopyable.hpp>

#include <set>
#include <string>

// FIXME Debug
#include <graphics/contexts/DrawContext.hpp>

namespace phy
{
class Body;
class Space;
}

namespace logic
{

class World;

class Entity : public boost::noncopyable
{
private:
    // Identifiers
    std::string     _name;

    sys::FTime      _next_update    = 0;

    // State
    bool            _spawned        = false;

    // Sub-systems lists
    std::set<gfx::Drawable *>       _drawables;
    std::set<gfx::Light *>          _lights;
    std::set<gfx::ShadowVolume *>   _shadows;
    std::set<phy::Body *>           _bodies;

    // Lua entity
    mutable LuaObject               _lua_object;


public:
                Entity();

    virtual     ~Entity();

    const std::string & getName() const             { return _name; }
    void        setName(const std::string & name)   { _name = name; }

    // FIXME Debug
    void        testRender(gfx::DrawContext & draw);

    // Values and events
    Variant     getValue(const std::string & key) const;
    void        setValue(const std::string & key, const Variant & value);

    void        sendEvent(const std::string & event, const Variant & param = Variant());

    LuaObject & getLuaObject() const                { return _lua_object; }


protected:
    // Shorthands for systems
    World &     getWorld();
    phy::Space & getSpace();
    Lua &       getLua() const;

    // Sub-systems
    void        addDrawable(gfx::Drawable * drawable);
    void        removeDrawable(gfx::Drawable * drawable);

    void        addLight(gfx::Light * light);
    void        removeLight(gfx::Light * light);

    void        addShadow(gfx::ShadowVolume * shadow);
    void        removeShadow(gfx::ShadowVolume * shadow);

    void        addBody(phy::Body * body);
    void        removeBody(phy::Body * body);

    // Functions to be overloaded
    virtual void    doSpawn()       {}
    virtual void    doDespawn()     {}

    virtual void    doUpdate()      {}

    // Default behavior is to get / set from lua object
    virtual Variant doGetValue(const std::string & key) const;
    virtual void    doSetValue(const std::string & key, const Variant & value);

    virtual void    doEvent(const std::string & event, const Variant & param);


private:
    friend class World;

    void        spawn();
    void        despawn();

    void        update();


};
// class Entity

}
// namespace logic

#endif // _IWBAN_ENTITY_HPP_
