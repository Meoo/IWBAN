/**
 * @file   Sprite.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SPRITE_HPP_
#define _IWBAN_SPRITE_HPP_

#include <Global.hpp>

#include <graphics/Drawable.hpp>

#include <SFML/Graphics.hpp>

namespace gfx
{

class Sprite : public Drawable
{
private:
    // Data members
    sf::Sprite _sprite;

public:
    virtual void draw(gfx::Renderer & renderer)
    {
        renderer.draw(_sprite);
    }

};
// class Sprite

}
// namespace gfx

#endif // _IWBAN_SPRITE_HPP_
