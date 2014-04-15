/**
 * @file   Box.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BOX_HPP_
#define _IWBAN_BOX_HPP_

#include <Global.hpp>

#include <physics/Shape.hpp>

namespace phy
{

class Box : public Shape
{
private:
    // Data members
    ut::Rectangle   _bbox;


public:
    // Constructor
    Box(const ut::Rectangle & box)
        : _bbox(box)
    {
    }

    // Virtual functions
    virtual const ut::Rectangle & getBoundingBox() const
    {
        return _bbox;
    }

};
// class Box

}
// namespace phy

#endif // _IWBAN_BOX_HPP_
