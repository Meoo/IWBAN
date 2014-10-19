/**
 * @file   Scene.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SCENE_HPP_
#define _IWBAN_SCENE_HPP_

#include <Global.hpp>

#include <boost/noncopyable.hpp>

#include <list>

namespace gfx
{

class Drawable;
class Light;
class Renderer;
class ShadowVolume;

class Scene : public boost::noncopyable
{
private:
    typedef std::list<Drawable *>       DrawableList;
    typedef std::list<Light *>          LightList;
    typedef std::list<ShadowVolume *>   ShadowVolumeList;


    // Data members
    DrawableList        _drawables;
    LightList           _lights;
    ShadowVolumeList    _shadows;


public:
    // Functions
    void add(Drawable * drawable);
    void remove(Drawable * drawable) noexcept;

    void add(Light * light);
    void remove(Light * light) noexcept;

    void add(ShadowVolume * shadow);
    void remove(ShadowVolume * shadow) noexcept;


    void draw(Renderer & renderer) const;

};
// class Scene

}
// namespace gfx

#endif // _IWBAN_SCENE_HPP_
