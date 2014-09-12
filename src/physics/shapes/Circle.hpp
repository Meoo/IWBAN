/**
 * @file   Circle.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CIRCLE_HPP_
#define _IWBAN_CIRCLE_HPP_

#include <Global.hpp>

#include <physics/Shape.hpp>

namespace phy
{

class Circle : public Shape
{
private:
    // Data members
    float           _radius;
    ut::Rectangle   _bbox;


public:
    // Constructor
    Circle(float radius)
        : _radius(radius)
    {
        _bbox.x = - radius;
        _bbox.y = - radius;
        _bbox.w = radius * 2;
        _bbox.h = radius * 2;
    }

    // Virtual functions
    const ut::Rectangle & getBoundingBox() const override
    {
        return _bbox;
    }

};
// class Circle

}
// namespace phy

#endif // _IWBAN_CIRCLE_HPP_
