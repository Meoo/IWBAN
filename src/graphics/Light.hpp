/**
 * @file   Light.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LIGHT_HPP_
#define _IWBAN_LIGHT_HPP_

#include <Global.hpp>

#include <boost/intrusive/set.hpp>

namespace gfx
{

class Light : public boost::intrusive::set_base_hook<>
{
public:
    typedef boost::intrusive::set<Drawable> List;


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
