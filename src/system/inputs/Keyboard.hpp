/**
 * @file   Keyboard.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_KEYBOARD_HPP_
#define _IWBAN_KEYBOARD_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>

#include <SFML/Window.hpp>

namespace sys
{

class Keyboard
{
private:
    // Data members
    Controls &  _controls;

    ActionId    _key_to_action[sf::Keyboard::KeyCount];


public:
    // Constructor
    Keyboard(Controls & controls);

    void onKeyPressed(sf::Keyboard::Key key);
    void onKeyReleased(sf::Keyboard::Key key);

};
// class Keyboard

// ---- ---- ---- ----

inline
Keyboard & getKeyboard()
{
    static Keyboard s_keyboard(getControls());
    return s_keyboard;
}

}
// namespace sys

#endif // _IWBAN_KEYBOARD_HPP_
