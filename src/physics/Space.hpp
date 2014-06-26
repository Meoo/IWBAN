/**
 * @file   Space.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SPACE_HPP_
#define _IWBAN_SPACE_HPP_

#include <Global.hpp>

#include <physics/Body.hpp>

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
    typedef std::function<void(const Body &, const Body &)> PairCallback;

    typedef std::function<void(const Body &)> RayCallback;
    typedef std::function<void(const Body &)> RectangleCallback;
    typedef std::function<void(const Body &)> ShapeCallback;

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

    /**
     * Refresh the space.
     *
     * This function should be called after the bodies have been updated.
     */
    void refresh();

    /**
     * Find every pair of bodies that can collide.
     *
     * This function ensure that every pair passed to the callback
     * has been checked with Body::canCollide.
     */
    void computePairs(const PairCallback & callback) const;

    void testRay(const ut::Vector & begin, const ut::Vector & end,
                 const RayCallback & callback) const;

    void testRectangle(const ut::Rectangle & rect,
                       const RectangleCallback & callback) const;

    /**
     * Find bodies that can collide given shape.
     *
     * This function ensure taht every body passed to the callback
     * has been checked with Body::canCollide.
     */
    void testShape(const Shape & shape,
                   const ShapeCallback & callback) const;

#ifndef NDEBUG
    void drawDebug(gfx::DebugContext & debug_context) const;
#endif

};
// class Space

}
// namespace phy

#endif // _IWBAN_SPACE_HPP_
