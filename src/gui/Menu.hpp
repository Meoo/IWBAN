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

    std::size_t     _selected   = -1u;

    bool            _centered   = false;
    unsigned        _spacing    = 0;


public:
    // Constructor
    Menu();

    // Virtual destructor
    virtual ~Menu();

    // Functions
    virtual void draw(gfx::DrawContext & context) const;

    // Elements added to a Menu are owned, and will be deleted when
    // the menu is released.
    void add(Element * element);

    const ut::Vector & getPosition() const  { return _position; }
    virtual void setPosition(const ut::Vector & position) { _position = position; }

    ut::Vector getSize() const              { return _size; }
    void setSize(const ut::Vector & size)   { _size = size; }

    void setCentered(bool centered)         { _centered = centered; updateChilds(); }

    virtual void dispatchAction(sys::ActionId action);


private:
    // Private function
    void updateChilds();

};
// class Menu

}
// namespace gui

#endif // _IWBAN_MENU_HPP_
