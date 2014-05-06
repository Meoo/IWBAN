/**
 * @file   Menu.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MENU_HPP_
#define _IWBAN_MENU_HPP_

#include <Global.hpp>

#include <gui/Element.hpp>

#include <vector>

namespace gui
{

// A menu organized as a vertical list
class Menu : public Element
{
private:
    // Data members
    ut::Vector      _position;
    ut::Vector      _size;

    std::vector<Element *> _elements;

    std::size_t     _first_visible;
    std::size_t     _selected;

    bool            _centered;
    unsigned        _spacing;


public:
    // Functions
    void draw(gfx::DrawContext & context);

    void add(Element * element);

    const ut::Vector & getPosition() const  { return _position; }
    ut::Vector getSize() const              { return _size; }

    void setCentered(bool centered)         { _centered = centered; }

};
// class Menu

}
// namespace gui

#endif // _IWBAN_MENU_HPP_
