/**
 * @file   Rectangle.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_RECTANGLE_HPP_
#define _IWBAN_RECTANGLE_HPP_

#include <Global.hpp>

#include <utils/Vector.hpp>

#include <SFML/Graphics/Rect.hpp>

namespace ut
{

class Rectangle
{
public:
    typedef float Float;


    // Data members
    Float left      = 0;
    Float top       = 0;
    Float right     = 0;
    Float bottom    = 0;


    // Constructors
    Rectangle() = default;

    constexpr
    Rectangle(Float left, Float top, Float right, Float bottom)
        : left(left), top(top), right(right), bottom(bottom) {}

    template<typename U> constexpr
    Rectangle(const sf::Rect<U> & rect)
        : left(rect.left), top(rect.top),
          right(rect.left + rect.width), bottom(rect.top + rect.height) {}


    // Conversion & static constructors
    template<typename U> constexpr
    operator sf::Rect<U>() const
    {
        return sf::Rect<U>(left, top, right - left, bottom - top);
    }

    static constexpr
    Rectangle fromSize(float x, float y, float w, float h)
    {
        return Rectangle(x, y, x + w, y + h);
    }

    static constexpr
    Rectangle fromSize(const Vector & origin, const Vector & size)
    {
        return Rectangle(origin.x, origin.y,
                         origin.x + size.x, origin.y + size.y);
    }


    // Functions
    void grow(Float margin)
    {
        left    -= margin;
        top     -= margin;
        right   += margin;
        bottom  += margin;
    }

    void expand(Float x, Float y)
    {
        left    = std::min(left, x);
        top     = std::min(top, y);
        right   = std::max(right, x);
        bottom  = std::max(bottom, y);
    }

    void expand(const Vector & point)
    {
        left    = std::min(left, point.x);
        top     = std::min(top, point.y);
        right   = std::max(right, point.x);
        bottom  = std::max(bottom, point.y);
    }

    void expand(const Rectangle & rectangle)
    {
        left    = std::min(left, rectangle.left);
        top     = std::min(top, rectangle.top);
        right   = std::max(right, rectangle.right);
        bottom  = std::max(bottom, rectangle.bottom);
    }

    void translate(Float x, Float y)
    {
        left    += x;
        right   += x;
        top     += y;
        bottom  += y;
    }

    void translate(const Vector & vector)
    {
        left    += vector.x;
        right   += vector.x;
        top     += vector.y;
        bottom  += vector.y;
    }

    Rectangle computeUnion(const Rectangle & rectangle) const
    {
        return Rectangle(std::min(left, rectangle.left),
                         std::min(top, rectangle.top),
                         std::max(right, rectangle.right),
                         std::max(bottom, rectangle.bottom));
    }

    Rectangle computeIntersection(const Rectangle & rectangle) const
    {
        return Rectangle(std::max(left, rectangle.left),
                         std::max(top, rectangle.top),
                         std::min(right, rectangle.right),
                         std::min(bottom, rectangle.bottom));
    }

    constexpr
    Float getWidth() const
    {
        return right - left;
    }

    constexpr
    Float getHeight() const
    {
        return bottom - top;
    }

    constexpr
    ut::Vector getSize() const
    {
        return ut::Vector(getWidth(), getHeight());
    }

    constexpr
    Float getVolume() const
    {
        return getWidth() * getHeight();
    }


    // Predicates
    constexpr
    bool isEmpty() const
    {
        return left >= right || top >= bottom;
    }

    constexpr
    bool isContaining(Float x, Float y) const
    {
        return x >= left && x <= right && y >= top && y <= bottom;
    }

    constexpr
    bool isContaining(const Vector & point) const
    {
        return point.x >= left && point.x <= right
            && point.y >= top && point.y <= bottom;
    }

    constexpr
    bool isContaining(const Rectangle & rectangle) const
    {
        return rectangle.left >= left && rectangle.right <= right
            && rectangle.top >= top && rectangle.bottom <= bottom;
    }

    bool isIntersecting(const Rectangle & rectangle) const
    {
        // TODO Can be optimized a little?
        return !computeIntersection(rectangle).isEmpty();
    }

};
// class Rectangle

}
// namespace ut

#endif // _IWBAN_RECTANGLE_HPP_
