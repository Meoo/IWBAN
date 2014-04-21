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

namespace phy
{

class Shape
{
public:
    // Virtual destructor
    virtual ~Shape() {}

    // Virtual functions
    virtual const ut::Rectangle & getBoundingBox() const = 0;

};
// class Shape

// TODO Change name?
// TODO Real implementation
// offset is position of second from first
inline
bool collideShapes(const Shape * first, const Shape * second,
                   const ut::Vector & offset, CollisionData & result)
{
    // TODO collide
    const ut::Rectangle & first_bb = first->getBoundingBox();
    ut::Rectangle second_bb = second->getBoundingBox();
    second_bb.x += offset.x;
    second_bb.y += offset.y;

    ut::Rectangle rec = ut::intersect(first_bb, second_bb);

    if (rec.isEmpty())
        return false;

    ut::Vector orig;
    orig.x = rec.x + rec.w / 2;
    orig.y = rec.y + rec.h / 2;
    result.origin = orig;

    if (first_bb.x > second_bb.x)
        result.intersect.x = rec.w;
    else
        result.intersect.x = - rec.w;

    if (first_bb.y > second_bb.y)
        result.intersect.y = rec.h;
    else
        result.intersect.y = - rec.h;

    return true;
}

}
// namespace phy

#endif // _IWBAN_SHAPE_HPP_
