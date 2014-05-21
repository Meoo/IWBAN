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

    sf::Color               _idle_color     = sf::Color(152, 152, 152);
    sf::Color               _selected_color = sf::Color::White;
    sf::Color               _disabled_color = sf::Color(96, 96, 96);


public:
    // Constructors
    Choice() { setTextColor(_idle_color); }

    explicit Choice(const std::string & string_key)
        : Label(string_key) { setTextColor(_idle_color); }

    // Functions
    void enable();
    void disable();

    bool isEnabled() const  { return _enabled; }

    virtual void select();
    virtual void deselect();

    virtual bool isSelectable() const { return isEnabled(); }

    void setAction(const std::function<void()> & func) { _action = func; }

    virtual void dispatchAction(sys::ActionId action);

    void setIdleColor(const sf::Color & color);
    void setSelectedColor(const sf::Color & color);
    void setDisabledColor(const sf::Color & color);

    void updateColor();

};
// class Choice

}
// namespace gui

#endif // _IWBAN_CHOICE_HPP_
