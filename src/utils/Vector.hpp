/**
 * @file   Vector.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_VECTOR_HPP_
#define _IWBAN_VECTOR_HPP_

#include <Global.hpp>

namespace ut
{

// TODO Sort Vector / Rect classes out


class Vector
{
public:
    constexpr Vector() : x(0), y(0) {}
    constexpr Vector(float x, float y) : x(x), y(y) {}

    float x;
    float y;
};

constexpr Vector operator+(const Vector & a, const Vector & b)
{
    return Vector(a.x + b.x, a.y + b.y);
}

constexpr Vector operator-(const Vector & a, const Vector & b)
{
    return Vector(a.x - b.x, a.y - b.y);
}

template<typename T>
constexpr Vector operator*(const Vector & a, const T & b)
{
    return Vector(a.x * b, a.y * b);
}

template<typename T>
constexpr Vector operator/(const Vector & a, const T & b)
{
    return Vector(a.x / b, a.y / b);
}

inline Vector & operator+=(Vector & a, const Vector & b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

inline Vector & operator-=(Vector & a, const Vector & b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

}
// namespace ut

#endif // _IWBAN_VECTOR_HPP_
