/**
 * @file   Rectangle.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_RECTANGLE_HPP_
#define _IWBAN_RECTANGLE_HPP_

#include <Global.hpp>

#include <SFML/Graphics/Rect.hpp>

namespace ut
{

class Rectangle
{
public:
    constexpr Rectangle() {}
    constexpr Rectangle(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    constexpr Rectangle(const sf::FloatRect & rec)
        : x(rec.left), y(rec.top), w(rec.width), h(rec.height) {}
    operator sf::FloatRect() const { return sf::FloatRect(x, y, w, h); }

    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;

    bool isEmpty()
    {
        return w <= 0 || h <= 0;
    }
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
    // TODO Can be optimized a little ?
    return !intersect(a, b).isEmpty();
}

}
// namespace ut

#endif // _IWBAN_RECTANGLE_HPP_
