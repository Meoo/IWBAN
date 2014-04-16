/**
 * @file   Rectangle.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_RECTANGLE_HPP_
#define _IWBAN_RECTANGLE_HPP_

namespace ut
{

class Rectangle
{
public:
    float x;
    float y;
    float w;
    float h;
};
// class Rectangle

inline
Rectangle intersect(const Rectangle & a, const Rectangle & b)
{
    Rectangle ret;

    if (a.x > b.x)
        ret.x = a.x;
    else
        ret.x = b.x;

    if (a.y > b.y)
        ret.y = a.y;
    else
        ret.y = b.y;

    if (a.x + a.w < b.x + b.w)
        ret.w = (a.x + a.w) - ret.x;
    else
        ret.w = (b.x + b.w) - ret.x;

    if (a.y + a.h < b.y + b.h)
        ret.h = (a.y + a.h) - ret.y;
    else
        ret.h = (b.y + b.h) - ret.y;

    return ret;
}

inline
bool hasIntersection(const Rectangle & a, const Rectangle & b)
{
    // TODO Can be optimized a little.
    Rectangle i = intersect(a, b);
    return i.w > 0 && i.h > 0;
}

}
// namespace ut

#endif // _IWBAN_RECTANGLE_HPP_
