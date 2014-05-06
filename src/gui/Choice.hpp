/**
 * @file   Choice.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CHOICE_HPP_
#define _IWBAN_CHOICE_HPP_

#include <Global.hpp>

#include <gui/Label.hpp>

namespace gui
{

// A choice in a menu
class Choice : public Label
{
private:
    // Data members


public:
    // Constructor
    explicit Choice(const std::string & string_key) : Label(string_key) {}

    // Functions
    void enable()           { _enabled = true; }
    void disable()          { _enabled = false; }

    bool isEnabled() const  { return _enabled; }

    virtual void select()   { Label::select(); setTextColor(sf::Color::Blue); }
    virtual void deselect() { Label::deselect(); setTextColor(sf::Color::White); }

    virtual bool isSelectable() const { return isEnabled(); }

};
// class Choice

}
// namespace gui

#endif // _IWBAN_CHOICE_HPP_
