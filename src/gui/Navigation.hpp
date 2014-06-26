/**
 * @file   Navigation.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_NAVIGATION_HPP_
#define _IWBAN_NAVIGATION_HPP_

#include <Global.hpp>

#include <gui/Selectable.hpp>

#include <system/Controls.hpp>

#include <map>

namespace gui
{

class Navigation
{
public:
    class NavigationEntry
    {
    public:
        Selectable * up     = nullptr;
        Selectable * down   = nullptr;
        Selectable * left   = nullptr;
        Selectable * right  = nullptr;
    };
    // class NavigationEntry

    typedef std::map<Selectable *, NavigationEntry> NavigationTable;


private:
    // Data members
    Selectable *    _head       = nullptr;
    Selectable *    _selected   = nullptr;

    NavigationTable _table;


public:
    // Functions
    void    dispatchAction(sys::ActionId action);

    // Add connections
    void    addVertical(Selectable * up, Selectable * down);
    void    addHorizontal(Selectable * left, Selectable * right);

    // Mark an element as the element selected by default
    void    setHead(Selectable * head);

    // Deselect the current element and select the head
    void    reset();

};
// class Navigation

}
// namespace gui

#endif // _IWBAN_NAVIGATION_HPP_