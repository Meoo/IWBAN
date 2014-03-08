/**
 * @file   TMPVector.cpp
 * @author Bastien Brunnenstein
 */

// TODO Sort Vector / Rect classes out

typedef struct { float x; float y; } Vector;
typedef struct { float x; float y; float w; float h; } Rect;

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

