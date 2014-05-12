/**
 * @file   Choice.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CHOICE_HPP_
#define _IWBAN_CHOICE_HPP_

#include <Global.hpp>

#include <gui/Label.hpp>

#include <functional>

namespace gui
{

// A choice in a menu
class Choice : public Label
{
private:
    // Data members
    bool                    _enabled = true;
    std::function<void()>   _action;


public:
    // Constructor
    explicit Choice(const std::string & string_key) : Label(string_key) {}

    // Functions
    void enable()           { _enabled = true; }
    void disable()          { _enabled = false; }

    bool isEnabled() const  { return _enabled; }

    virtual void select()   { Label::select(); setTextColor(sf::Color::Green); }
    virtual void deselect() { Label::deselect(); setTextColor(sf::Color::White); }

    virtual bool isSelectable() const { return isEnabled(); }

    void setAction(const std::function<void()> & func) { _action = func; }

    virtual void dispatchAction(sys::ActionId action)
    {
        if (action == sys::ACT_ACCEPT && _action)
            _action();
    }

};
// class Choice

}
// namespace gui

#endif // _IWBAN_CHOICE_HPP_
