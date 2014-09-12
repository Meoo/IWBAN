/**
 * @file   Box.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BOX_HPP_
#define _IWBAN_BOX_HPP_

#include <Global.hpp>

#include <physics/Shape.hpp>

namespace phy
{

class Box : public Shape
{
private:
    // Data members
    ut::Rectangle   _bbox;

    IWBAN_DEBUG(mutable sf::RectangleShape d_shape);


public:
    // Constructor
    Box(const ut::Rectangle & box)
        : _bbox(box)
    {
        IWBAN_ASSERT(box.w > 0 && box.h > 0);

        IWBAN_DEBUG(d_shape.setSize(sf::Vector2f(_bbox.w, _bbox.h)));
        IWBAN_DEBUG(d_shape.setFillColor(sf::Color::Transparent));
        IWBAN_DEBUG(d_shape.setOutlineThickness(-1));
    }

    // Virtual functions
    const ut::Rectangle & getBoundingBox() const override
    {
        return _bbox;
    }

#ifndef NDEBUG
    void drawDebug(gfx::DebugContext & debug_context,
                   const ut::Vector & origin, const sf::Color & color) const override
    {
        d_shape.setOutlineColor(color);
        d_shape.setPosition(origin.x + _bbox.x, origin.y + _bbox.y);

        debug_context.draw(d_shape);
    }
#endif

};
// class Box

}
// namespace phy

#endif // _IWBAN_BOX_HPP_
