/**
 * @file   Shape.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SHAPE_HPP_
#define _IWBAN_SHAPE_HPP_

#include <Global.hpp>

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

}
// namespace phy

#endif // _IWBAN_SHAPE_HPP_
