/**
 * @file   Selectable.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SELECTABLE_HPP_
#define _IWBAN_SELECTABLE_HPP_

#include <Global.hpp>

#include <gui/Element.hpp>

#include <utils/Rectangle.hpp>

namespace gui
{

class Selectable : public Element
{
public:
    // Virtual destructor
    virtual         ~Selectable() {}

    // Functions
    virtual bool    isEnabled() const { return true; }

    virtual void    select() {}
    virtual void    deselect() {}

    virtual ut::Rectangle getBounds() const = 0;

    virtual void    dispatchAction(sys::ActionId action) {}

};
// class Selectable

}
// namespace gui

#endif // _IWBAN_SELECTABLE_HPP_
