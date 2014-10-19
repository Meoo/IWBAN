/**
 * @file   Drawable.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_DRAWABLE_HPP_
#define _IWBAN_DRAWABLE_HPP_

#include <Global.hpp>

#include <utils/Rectangle.hpp>

#include <SFML/Graphics.hpp>

#include <boost/noncopyable.hpp>

namespace gfx
{

class Scene;

// TODO Remove sf::Drawable inheritance?
class Drawable : public sf::Drawable, public boost::noncopyable
{
private:
    // Data members
    Scene * _scene      = nullptr;

    int     _depth      = 0;


public:
    // Virtual destructor
    virtual ~Drawable();

    // Getters / setters
    int     getDepth() const    { return _depth; }
    void    setDepth(int depth) { _depth = depth; }

    virtual ut::Rectangle getBounds() const = 0;


protected:
    Scene * getScene()          { return _scene; }


private:
    friend class Scene;

    void    init(Scene * scene);
    void    deinit();

};
// class Drawable

}
// namespace gfx

#endif // _IWBAN_DRAWABLE_HPP_
