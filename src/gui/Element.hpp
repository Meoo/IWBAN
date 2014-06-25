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
private:
    // Data members
    bool    _selected = false;


public:
    // Virtual destructor
    virtual ~Element() {}

    // Functions
    virtual void    draw(gfx::GuiContext & context) const = 0;

    virtual ut::Vector getSize() const = 0;

    virtual void    setPosition(const ut::Vector & position) = 0;

    virtual bool    isSelectable() const    { return false; }
    bool            isSelected() const      { return _selected; }

    virtual void    select()   { BOOST_ASSERT(isSelectable()); _selected = true; }
    virtual void    deselect() { BOOST_ASSERT(isSelectable()); _selected = false; }

    virtual void    dispatchAction(sys::ActionId action) {}

};
// class Element

}
// namespace gui

#endif // _IWBAN_ELEMENT_HPP_
