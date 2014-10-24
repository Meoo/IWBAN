/**
 * @file   Vector.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_VECTOR_HPP_
#define _IWBAN_VECTOR_HPP_

#include <Global.hpp>

#include <utils/StreamIO.hpp>

#include <SFML/System/Vector2.hpp>

#include <cmath>

namespace ut
{

class Vector
{
public:
    constexpr Vector() {}
    constexpr Vector(float x, float y) : x(x), y(y) {}

    constexpr Vector(const sf::Vector2f & vec) : x(vec.x), y(vec.y) {}
    operator sf::Vector2f() const { return sf::Vector2f(x, y); }

    float x = 0;
    float y = 0;

    constexpr bool isNull() const
    {
        return !x && !y;
    }

    constexpr float getLength() const
    {
        return std::sqrt(x * x + y * y);
    }
};

constexpr Vector operator+(const Vector & a, const Vector & b)
{
    return Vector(a.x + b.x, a.y + b.y);
}

constexpr Vector operator-(const Vector & a, const Vector & b)
{
    return Vector(a.x - b.x, a.y - b.y);
}

constexpr Vector operator-(const Vector & a)
{
    return Vector(- a.x, - a.y);
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

constexpr float dot(const Vector & a, const Vector & b)
{
    return a.x * b.x + a.y * b.y;
}

// Project a on b
inline Vector project(const Vector & a, const Vector & b)
{
    // TODO Throw instead
    IWBAN_PRE(!b.isNull());

    if (a.isNull())
        return Vector();

    float prod = dot(a, b) / (b.x * b.x + b.y * b.y);
    return Vector(b.x * prod, b.y * prod);
}

inline Vector normalize(const Vector & a)
{
    // TODO Throw instead
    IWBAN_PRE(!a.isNull());

    float len = a.getLength();
    return Vector(a.x / len, a.y / len);
}

// ---- ---- ---- ----

/**
 * Read a vector from a binary stream.
 */
template<> inline
Vector read<Vector>(std::istream & stream)
{
    float x = read<float>(stream);
    float y = read<float>(stream);
    return Vector(x, y);
}

/**
 * Write a vector to a binary stream.
 */
template<> inline
void write<Vector>(std::ostream & stream, const Vector & value)
{
    write<float>(stream, value.x);
    write<float>(stream, value.y);
}

}
// namespace ut

#endif // _IWBAN_VECTOR_HPP_
