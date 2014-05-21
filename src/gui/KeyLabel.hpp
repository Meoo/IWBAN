/**
 * @file   KeyLabel.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_KEYLABEL_HPP_
#define _IWBAN_KEYLABEL_HPP_

#include <Global.hpp>

#include <gui/Choice.hpp>

#include <SFML/Window.hpp>

namespace gui
{

// A label displaying a keyboard key name
class KeyLabel : public Choice
{
public:
    // Constructors
    KeyLabel() {}

    explicit KeyLabel(sf::Keyboard::Key key) { setKey(key); }

    // Function
    void setKey(sf::Keyboard::Key key)
    {
        char buffer[16];
        std::snprintf(buffer, sizeof(buffer), "keyboard.%u", key);
        setText(buffer);
    }

};
// class Label

}
// namespace gui

#endif // _IWBAN_KEYLABEL_HPP_
