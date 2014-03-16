/**
 * @file   Background.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BACKGROUND_HPP_
#define _IWBAN_BACKGROUND_HPP_

#include <Global.hpp>

namespace gfx
{

class Background
{
public:
    class Element
    {
    public:
        // Data members
        // TODO Texture

        sf::Sprite sprite;

        float rotation;
        float rotation_speed;

        float scale_x;
        float scale_y;

    };
    // class Element

    // ---- ---- ---- ----

    void    render(gfx::Renderer & renderer) const;

};
// class Background

}
// namespace gfx

#endif // _IWBAN_BACKGROUND_HPP_
