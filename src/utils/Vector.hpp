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
    float x;
    float y;
};

inline Vector operator+(const Vector & a, const Vector & b)
{
    Vector v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    return v;
}

inline Vector operator-(const Vector & a, const Vector & b)
{
    Vector v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    return v;
}

template<typename T>
inline Vector operator*(const Vector & a, const T & b)
{
    Vector v;
    v.x = a.x * b;
    v.y = a.y * b;
    return v;
}

template<typename T>
inline Vector operator/(const Vector & a, const T & b)
{
    Vector v;
    v.x = a.x / b;
    v.y = a.y / b;
    return v;
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
