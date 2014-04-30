/**
 * @file   MenuElement.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MENUELEMENT_HPP_
#define _IWBAN_MENUELEMENT_HPP_

#include <Global.hpp>

#include <graphics/contexts/DrawContext.hpp>

#include <utils/Vector.hpp>

namespace gfx
{

class MenuElement
{
private:
    // Data members
    ut::Vector      _size;

    bool            _selected;


public:
    // Virtual destructor
    virtual ~MenuElement() {}

    // Functions
    virtual void draw(DrawContext & context, const ut::Vector & origin) = 0;

    const ut::Vector & getSize() const  { return _size; }

    bool isSelectable() const;

    void select()       { _selected = true; }
    void deselect()     { _selected = false; }
    bool isSelected() const;

};
// class MenuElement

}
// namespace gfx

#endif // _IWBAN_MENUELEMENT_HPP_
