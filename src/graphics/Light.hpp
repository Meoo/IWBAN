/**
 * @file   Light.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LIGHT_HPP_
#define _IWBAN_LIGHT_HPP_

#include <Global.hpp>

#include <utils/Rectangle.hpp>
#include <utils/Vector.hpp>

#include <SFML/Graphics.hpp>

#include <boost/noncopyable.hpp>

namespace gfx
{

class Scene;

// TODO Remove sf::Drawable inheritance?
class Light : public sf::Drawable, public boost::noncopyable
{
private:
    // Data members
    Scene *     _scene      = nullptr;

    bool        _cast_shadows;

    ut::Vector  _origin;


public:
    // Destructor
                 ~Light();

    // Getters
    const ut::Vector & getOrigin() const    { return _origin; }

    // TODO Light:getBounds
    ut::Rectangle getBounds() const         { return ut::Rectangle(); }


private:
    friend class Scene;

    Scene *     getScene()                  { return _scene; }

    void        init(Scene * scene);
    void        deinit();

};
// class Light

}
// namespace gfx

#endif // _IWBAN_LIGHT_HPP_
