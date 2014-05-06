/**
 * @file   Menu.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MENU_HPP_
#define _IWBAN_MENU_HPP_

#include <Global.hpp>

#include <graphics/contexts/DrawContext.hpp>

#include <utils/Vector.hpp>

#include <vector>

namespace gui
{

// A menu organized as a vertical list
class Menu
{
public:
    class Element
    {
    private:
        // Data members
        ut::Vector  _size;
        bool        _selected;


    public:
        // Virtual destructor
        virtual ~Element() {}

        // Functions
        virtual void draw(gfx::DrawContext & context, const ut::Vector & origin) = 0;

        const ut::Vector & getSize() const  { return _size; }

        virtual bool isSelectable() const;

        void select()           { _selected = true; }
        void deselect()         { _selected = false; }
        bool isSelected() const { return _selected; }

    };
    // class Element


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
    const ut::Vector & getSize() const      { return _size; }

    void setCentered(bool centered)         { _centered = centered; }

};
// class Menu

}
// namespace gui

#endif // _IWBAN_MENU_HPP_
