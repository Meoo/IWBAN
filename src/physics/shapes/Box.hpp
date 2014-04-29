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


public:
    // Constructor
    Box(const ut::Rectangle & box)
        : _bbox(box)
    {
        BOOST_ASSERT(box.w > 0 && box.h > 0);
    }

    // Virtual functions
    virtual const ut::Rectangle & getBoundingBox() const
    {
        return _bbox;
    }

#ifndef NDEBUG
    virtual void drawDebug(gfx::DebugContext & debug_context,
                           const ut::Vector & origin, const sf::Color & color) const
    {
        sf::RectangleShape shape(sf::Vector2f(_bbox.w, _bbox.h));

        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(-1);
        shape.setOutlineColor(color);

        shape.setPosition(origin.x + _bbox.x, origin.y + _bbox.y);

        debug_context.draw(shape);
    }
#endif

};
// class Box

}
// namespace phy

#endif // _IWBAN_BOX_HPP_
