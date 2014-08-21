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

        Selectable * slave  = nullptr;
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

    void    dispatchMouseMove(const ut::Vector & position);
    void    dispatchMouseClick(const ut::Vector & position);

    // Add connections
    void    addVertical(Selectable * up, Selectable * down);
    void    addHorizontal(Selectable * left, Selectable * right);

    void    addSlave(Selectable * master, Selectable * slave);

    // Mark an element as the element selected by default
    void    setHead(Selectable * head);

    // Deselect the current element and select the head
    void    reset();


private:
    void    select(Selectable * elem);
    void    deselect();

};
// class Navigation

}
// namespace gui

#endif // _IWBAN_NAVIGATION_HPP_
