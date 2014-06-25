/**
 * @file   Separator.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SEPARATOR_HPP_
#define _IWBAN_SEPARATOR_HPP_

#include <Global.hpp>

#include <gui/Element.hpp>

namespace gui
{

class Separator : public Element
{
private:
    // Data members
    ut::Vector          _position;
    ut::Vector          _size;


public:
    // Constructor
    Separator(const ut::Vector & size) : _size(size) {}

    // Virtual destructor
    virtual ~Separator() {}

    // Functions
    virtual void draw(gfx::GuiContext & context) const {}

    virtual ut::Vector getSize() const      { return _size; }
    void setSize(const ut::Vector & size)   { _size = size; }

    virtual void setPosition(const ut::Vector & position) { _position = position; }

};
// class Separator

}
// namespace gui

#endif // _IWBAN_SEPARATOR_HPP_
