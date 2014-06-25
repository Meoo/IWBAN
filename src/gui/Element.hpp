/**
 * @file   Element.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ELEMENT_HPP_
#define _IWBAN_ELEMENT_HPP_

#include <Global.hpp>

#include <graphics/contexts/GuiContext.hpp>

#include <system/Controls.hpp>

#include <utils/Vector.hpp>

namespace gui
{

class Element
{
public:
    // Virtual destructor
    virtual         ~Element() {}

    // Functions
    virtual void    draw(gfx::GuiContext & context) const = 0;

    virtual void    setPosition(const ut::Vector & position) = 0;

    virtual void    refresh() {}

};
// class Element

}
// namespace gui

#endif // _IWBAN_ELEMENT_HPP_
