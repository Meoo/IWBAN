/**
 * @file   MenuList.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MENULIST_HPP_
#define _IWBAN_MENULIST_HPP_

#include <Global.hpp>

#include <graphics/contexts/DrawContext.hpp>

#include <utils/Vector.hpp>

#include <vector>

namespace gfx
{

class MenuElement;

class MenuList
{
private:
    // Data members
    ut::Vector      _position;
    ut::Vector      _size;

    std::vector<MenuElement *> _elements;

    std::size_t     _first_visible;
    std::size_t     _selected;

    bool            _centered;


public:
    // Functions
    void draw(DrawContext & context);

    void add(MenuElement * element);

    const ut::Vector & getPosition() const  { return _position; }
    const ut::Vector & getSize() const      { return _size; }

    void setCentered(bool centered)         { _centered = centered; }

};
// class MenuList

}
// namespace gfx

#endif // _IWBAN_MENULIST_HPP_
