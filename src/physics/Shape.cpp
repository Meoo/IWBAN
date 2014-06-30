/**
 * @file   Shape.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/Shape.hpp>

namespace phy
{

bool Shape::collide(const Shape & first, const Shape & secnd,
                    const ut::Vector & offset, CollisionData & result)
{
    // TODO collide
    const ut::Rectangle & first_bb = first.getBoundingBox();
    ut::Rectangle         secnd_bb = secnd.getBoundingBox();
    secnd_bb.x += offset.x;
    secnd_bb.y += offset.y;

    ut::Rectangle rec = ut::intersect(first_bb, secnd_bb);

    if (rec.isEmpty())
        return false;

    ut::Vector orig;
    orig.x = rec.x + rec.w / 2;
    orig.y = rec.y + rec.h / 2;
    result.origin = orig;

    if (first_bb.x > secnd_bb.x)
        result.mtv.x = rec.w;
    else
        result.mtv.x = - rec.w;

    if (first_bb.y > secnd_bb.y)
        result.mtv.y = rec.h;
    else
        result.mtv.y = - rec.h;

    return true;
}

}
// namespace phy
