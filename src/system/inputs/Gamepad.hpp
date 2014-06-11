/**
 * @file   Gamepad.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_GAMEPAD_HPP_
#define _IWBAN_GAMEPAD_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>

#include <SFML/Window.hpp>

#include <iostream>
#include <functional>

namespace sys
{

class Gamepad
{
public:
    typedef unsigned                    Button;
    typedef std::function<void(Button)> HookFunction;


private:
    // Data members
    Controls &  _controls;

    ActionId    _button_to_action[sf::Joystick::ButtonCount];
    Button      _action_to_button[ACT_COUNT];

    float       _axis_threshold;

    bool        _up = false;
    bool        _down = false;
    bool        _left = false;
    bool        _right = false;

    HookFunction _catch_hook;


public:
    // Constructor
            Gamepad(Controls & controls);

    void    mapButtonToAction(Button button, ActionId action);

    void    unmapButton(Button button);
    void    unmapAction(ActionId button);

    void    reloadDefaults();

    unsigned getButtonFromAction(ActionId action);

    void    onButtonPressed(Button button);
    void    onButtonReleased(Button button);

    void    onAxisMoved(sf::Joystick::Axis axis, float position);

    float   getAxisThreshold() const { return _axis_threshold; }
    void    setAxisThreshold(float thresh) { _axis_threshold = thresh; }

    void    catchNextButton(const HookFunction & hook) { _catch_hook = hook; }

};
// class Keyboard

// ---- ---- ---- ----

// Stream operators are avaiable to serialize button bindings to configuration file
std::ostream & operator << (std::ostream & ostr, Gamepad & gamepad);
std::istream & operator >> (std::istream & istr, Gamepad & gamepad);

// ---- ---- ---- ----

inline
Gamepad & getGamepad()
{
    static Gamepad s_gamepad(getControls());
    return s_gamepad;
}

}
// namespace sys

#endif // _IWBAN_GAMEPAD_HPP_
