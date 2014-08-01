/**
 * @file   Light.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LIGHT_HPP_
#define _IWBAN_LIGHT_HPP_

#include <Global.hpp>

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

    // Virtual functions
    void draw(gfx::LightContext & context);


};
// class Light

}
// namespace gfx

#endif // _IWBAN_LIGHT_HPP_
