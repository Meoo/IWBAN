/**
 * @file   Space.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SPACE_HPP_
#define _IWBAN_SPACE_HPP_

#include <Global.hpp>

#include <physics/Body.hpp>
#include <physics/Shape.hpp>

#include <utils/Rectangle.hpp>
#include <utils/Vector.hpp>

#include <functional>
#include <set>

#ifndef NDEBUG
#  include <graphics/contexts/debug/DebugContext.hpp>
#endif

namespace phy
{

class Space
{
public:
    typedef std::function<void(Body &, Body &)> PairCallback;

    typedef std::function<void(Body &)> RayCallback;
    typedef std::function<void(Body &)> RectangleCallback;
    typedef std::function<void(Body &)> ShapeCallback;

    typedef std::set<Body *>    BodyList;


private:
    // Data members
    BodyList    _bodies;


public:
    // Constructor
         Space();

    // Functions
    void add(Body * body);
    void remove(Body * body);

    // TODO update
    void update(const sf::Time & delta, int passes);

    void testRay(const ut::Vector & begin, const ut::Vector & end,
                 const RayCallback & callback) const;

    void testRectangle(const ut::Rectangle & rect,
                       const RectangleCallback & callback) const;

    /**
     * Find bodies that can collide given shape.
     */
    void testShape(const Shape & shape,
                   const ShapeCallback & callback) const;

#ifndef NDEBUG
    void drawDebug(gfx::DebugContext & debug_context) const;
#endif

private:
    /**
     * Find every pair of bodies that could collide.
     *
     * This is a broad phase collision detection. Pairs found by this
     * function may not be able to collide.
     */
    void computePairs(const PairCallback & callback) const;

};
// class Space

}
// namespace phy

#endif // _IWBAN_SPACE_HPP_
