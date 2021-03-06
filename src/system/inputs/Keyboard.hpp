/**
 * @file   Keyboard.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_KEYBOARD_HPP_
#define _IWBAN_KEYBOARD_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>

#include <SFML/Window.hpp>

#include <iostream>
#include <functional>

namespace sys
{

class Keyboard
{
public:
    typedef std::function<void(sf::Keyboard::Key)> HookFunction;


private:
    // Data members
    Controls &          _controls;

    ActionId            _key_to_action[sf::Keyboard::KeyCount];
    sf::Keyboard::Key   _action_to_key[ACT_COUNT];

    HookFunction        _catch_hook;


public:
    // Constructor
            Keyboard(Controls & controls);

    void    mapKeyToAction(sf::Keyboard::Key key, ActionId action);

    void    unmapKey(sf::Keyboard::Key key);
    void    unmapAction(ActionId action);

    void    reloadDefaults();

    sf::Keyboard::Key getKeyFromAction(ActionId action);

    void    onKeyPressed(sf::Keyboard::Key key);
    void    onKeyReleased(sf::Keyboard::Key key);

    void    catchNextKey(const HookFunction & hook) { _catch_hook = hook; }

};
// class Keyboard

// ---- ---- ---- ----

// Stream operators are avaiable to serialize key bindings to configuration file
std::ostream & operator << (std::ostream & ostr, Keyboard & keyboard);
std::istream & operator >> (std::istream & istr, Keyboard & keyboard);

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
