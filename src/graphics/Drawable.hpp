/**
 * @file   Drawable.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_DRAWABLE_HPP_
#define _IWBAN_DRAWABLE_HPP_

#include <Global.hpp>

#include <SFML/Graphics.hpp>

namespace gfx
{

class Drawable : public sf::Drawable
{
private:
    // Data members
    int     _depth;


public:
    // Virtual destructor
    virtual ~Drawable()     {}

    // Getters / setters
    int     getDepth() const    { return _depth; }
    void    setDepth(int depth) { _depth = depth; }

};
// class Drawable

}
// namespace gfx

#endif // _IWBAN_DRAWABLE_HPP_
