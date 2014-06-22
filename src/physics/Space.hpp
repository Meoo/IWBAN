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

#ifndef NDEBUG
#  include <graphics/contexts/debug/DebugContext.hpp>
#endif

namespace phy
{

class Space
{
public:
    // TODO const?
    typedef std::function<void(const Body &, const Body &)> PairCallback;

    typedef std::function<void(const Body &)> RayCallback;
    typedef std::function<void(const Body &)> RectangleCallback;


private:
    // Data members
    Body::List    _objects;


public:
    // Constructor
         Space();

    // Functions
    void add(Body * object);
    void remove(const Body * object);

    void step();

    void computePairs(const PairCallback & callback) const;

    void testRay(const ut::Vector & begin, const ut::Vector & end,
                 const RayCallback & callback) const;

    void testRectangle(const ut::Rectangle & rect,
                       const RectangleCallback & callback) const;

#ifndef NDEBUG
    void drawDebug(gfx::DebugContext & debug_context) const;
#endif

};
// class Space

}
// namespace phy

#endif // _IWBAN_SPACE_HPP_
