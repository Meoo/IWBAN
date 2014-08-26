/**
 * @file   Light.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LIGHT_HPP_
#define _IWBAN_LIGHT_HPP_

#include <Global.hpp>

#include <utils/Rectangle.hpp>
#include <utils/Vector.hpp>

namespace gfx
{

class Light
{
private:
    // Data members
    bool        _cast_shadows;

    ut::Vector  _origin;


public:
    // Virtual destructor
    virtual ~Light()     {}

    const ut::Vector & getOrigin() const { return _origin; }

    ut::Rectangle getBounds() const;

};
// class Light

}
// namespace gfx

#endif // _IWBAN_LIGHT_HPP_
