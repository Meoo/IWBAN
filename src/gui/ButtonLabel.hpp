/**
 * @file   ButtonLabel.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BUTTONLABEL_HPP_
#define _IWBAN_BUTTONLABEL_HPP_

#include <Global.hpp>

#include <gui/Choice.hpp>

#include <SFML/Window.hpp>

namespace gui
{

// A label displaying a gamepad button name
class ButtonLabel : public Choice
{
public:
    // Constructors
    ButtonLabel() {}

    explicit ButtonLabel(unsigned button) { setButton(button); }

    // Function
    void setButton(unsigned button)
    {
        char buffer[16];
        std::snprintf(buffer, sizeof(buffer), "gamepad.%u", button);
        setText(buffer);
    }

};
// class Label

}
// namespace gui

#endif // _IWBAN_BUTTONLABEL_HPP_
