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
#include <system/Time.hpp>

#include <boost/noncopyable.hpp>

#include <set>
#include <string>

// FIXME Debug
#include <graphics/contexts/DrawContext.hpp>

namespace phy
{
class Body;
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

    // Sub-systems lists
    std::set<gfx::Drawable *>       _drawables;
    std::set<gfx::Light *>          _lights;
    std::set<gfx::ShadowVolume *>   _shadows;
    std::set<phy::Body *>           _bodies;


public:
                Entity();

    virtual     ~Entity();

    const std::string & getName() const             { return _name; }
    void        setName(const std::string & name)   { _name = name; }

    // FIXME Debug
    void    testRender(gfx::DrawContext & draw);

    // Values and events
    /*template<typename T>
    const T &   getValue(const std::string & key);
    template<typename T>
    void        setValue(const std::string & key, const T & value);

    void        sendEvent(const std::string & event);
    template<typename T>
    void        sendEvent(const std::string & event, const T & param);*/


protected:
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
    virtual void    doUpdate()      {}

    /*virtual void    doGetValue()    {}
    virtual void    doSetValue()    {}

    virtual void    doEvent()       {}*/


private:
    friend class World;

    void        spawn()     { doSpawn(); }
    void        update()    { doUpdate(); }


};
// class Entity

}
// namespace logic

#endif // _IWBAN_ENTITY_HPP_
