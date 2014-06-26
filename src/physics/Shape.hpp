/**
 * @file   Shape.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SHAPE_HPP_
#define _IWBAN_SHAPE_HPP_

#include <Global.hpp>

#include <physics/CollisionData.hpp>
#include <physics/Shape.hpp>

#include <utils/Rectangle.hpp>

#ifndef NDEBUG
#  include <graphics/contexts/debug/DebugContext.hpp>
#  include <utils/Vector.hpp>
#endif

namespace phy
{

class Shape
{
public:
    // Virtual destructor
    virtual ~Shape() {}

    // Virtual functions
    virtual const ut::Rectangle & getBoundingBox() const = 0;

#ifndef NDEBUG
    virtual void drawDebug(gfx::DebugContext & debug_context,
                           const ut::Vector & origin, const sf::Color & color) const = 0;
#endif

    // Static function
    /**
     * Test collision of two shapes.
     *
     * @param offset position of second from first.
     */
    static bool collide(const Shape & first, const Shape & second,
                        const ut::Vector & offset, CollisionData & result);

};
// class Shape

}
// namespace phy

#endif // _IWBAN_SHAPE_HPP_
