/**
 * @file   Gamepad.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/GamepadConfig.hpp>

#include <system/inputs/Gamepad.hpp>

namespace sys
{

Gamepad::Gamepad(Controls & controls)
    : _controls(controls), _axis_threshold(IWBAN_GAMEPAD_AXIS_THRESH),
      _up(false), _down(false), _left(false), _right(false)
{
    // We use ACT_COUNT as an invalid id
    for (std::size_t i = 0; i < sf::Joystick::ButtonCount; ++i)
        _button_to_action[i] = ACT_COUNT;

    // We use KeyCount as an invalid id
    for (std::size_t i = 0; i < ACT_COUNT; ++i)
        _action_bindings[i] = sf::Joystick::ButtonCount;

    reloadDefaults();
}

void Gamepad::mapButtonToAction(unsigned button, ActionId action)
{
    BOOST_ASSERT(button < sf::Joystick::ButtonCount && action < ACT_COUNT);

    // Ignore fixed actions
    if (action == ACT_ACCEPT || action == ACT_CANCEL || action == ACT_MENU)
        return;

    // On gamepad, we need to make sure that actions are bound to something
    if (_action_bindings[action] != sf::Joystick::ButtonCount)
    {
        if (_button_to_action[button] != ACT_COUNT)
        {
            // Button is already in use
            // Swap with previously used button for our action
            unsigned act_prev_btn = _action_bindings[action];
            ActionId btn_prev_act = _button_to_action[button];
            _button_to_action[act_prev_btn] = btn_prev_act;
            _action_bindings[btn_prev_act] = act_prev_btn;
        }
        else
        {
            // Unmap previously used button for our action
            unmapButton(_action_bindings[action]);
        }
    }
    else if (_button_to_action[button] != ACT_COUNT)
        IWBAN_LOG_WARNING("Trying to map a button which is already bound\n");

    else
    {
        _button_to_action[button] = action;
        _action_bindings[action] = button;
    }
}

void Gamepad::unmapButton(unsigned button)
{
    BOOST_ASSERT(button < sf::Joystick::ButtonCount);

    // Remove from action mapping if required
    ActionId act = _button_to_action[button];

    if (_action_bindings[act] == button)
        _action_bindings[act] = sf::Joystick::ButtonCount;

    // Remove from key mapping
    _button_to_action[button] = ACT_COUNT;
}

void Gamepad::reloadDefaults()
{
    // Clean first
    for (std::size_t i = 0; i < ACT_COUNT; ++i)
    {
        if (_action_bindings[i] != sf::Joystick::ButtonCount)
        {
            _button_to_action[_action_bindings[i]] = ACT_COUNT;
            _action_bindings[i] = sf::Joystick::ButtonCount;
        }
    }

    // Dynamic configuration
    mapButtonToAction(IWBAN_GAMEPAD_JUMP, ACT_JUMP);
    mapButtonToAction(IWBAN_GAMEPAD_FIRE, ACT_FIRE);
    mapButtonToAction(IWBAN_GAMEPAD_RETRY, ACT_RETRY);
}

unsigned Gamepad::getButtonFromAction(ActionId action)
{
    BOOST_ASSERT(action < ACT_COUNT);

    return _action_bindings[action];
}

void Gamepad::onButtonPressed(unsigned button)
{
    BOOST_ASSERT(button < sf::Joystick::ButtonCount);

    if (_catch_hook)
    {
        _catch_hook(button);
        _catch_hook = nullptr;
        return;
    }

    // Fixed configuration
    switch (button)
    {
    case IWBAN_GAMEPAD_ACCEPT:
        _controls.getAction(ACT_ACCEPT).activate();
        break;

    case IWBAN_GAMEPAD_CANCEL:
        _controls.getAction(ACT_CANCEL).activate();
        break;

    case IWBAN_GAMEPAD_MENU:
        _controls.getAction(ACT_MENU).activate();
        break;

    default:
        break;
    }

    // Dynamic configuration
    ActionId id = _button_to_action[button];

    if (id != ACT_COUNT)
        _controls.getAction(id).activate();
}

void Gamepad::onButtonReleased(unsigned button)
{
    BOOST_ASSERT(button < sf::Joystick::ButtonCount);

    // Fixed configuration
    switch (button)
    {
    case IWBAN_GAMEPAD_ACCEPT:
        _controls.getAction(ACT_ACCEPT).deactivate();
        break;

    case IWBAN_GAMEPAD_CANCEL:
        _controls.getAction(ACT_CANCEL).deactivate();
        break;

    case IWBAN_GAMEPAD_MENU:
        _controls.getAction(ACT_MENU).deactivate();
        break;

    default:
        break;
    }

    // Dynamic configuration
    ActionId id = _button_to_action[button];

    if (id != ACT_COUNT)
        _controls.getAction(id).deactivate();
}

void Gamepad::onAxisMoved(sf::Joystick::Axis axis, float position)
{
    switch (axis)
    {
    // Horizontal
    case sf::Joystick::X:
    case sf::Joystick::PovX:

        // Right
        if (!_right && position >= _axis_threshold)
        {
            _controls.getAction(ACT_RIGHT).activate();
            _right = true;
        }
        else if (_right && position < _axis_threshold)
        {
            _controls.getAction(ACT_RIGHT).deactivate();
            _right = false;
        }

        // Left
        if (!_left && position <= - _axis_threshold)
        {
            _controls.getAction(ACT_LEFT).activate();
            _left = true;
        }
        else if (_left && position > - _axis_threshold)
        {
            _controls.getAction(ACT_LEFT).deactivate();
            _left = false;
        }
        break;

    // Vertical
    case sf::Joystick::PovY:
        position = - position; // For some reason PovY is inverted

    case sf::Joystick::Y:

        // Down
        if (!_down && position >= _axis_threshold)
        {
            _controls.getAction(ACT_DOWN).activate();
            _down = true;
        }
        else if (_down && position < _axis_threshold)
        {
            _controls.getAction(ACT_DOWN).deactivate();
            _down = false;
        }

        // Up
        if (!_up && position <= - _axis_threshold)
        {
            _controls.getAction(ACT_UP).activate();
            _up = true;
        }
        else if (_up && position > - _axis_threshold)
        {
            _controls.getAction(ACT_UP).deactivate();
            _up = false;
        }
        break;

    default:
        break;
    }
}

// ---- ---- ---- ----

std::ostream & operator << (std::ostream & ostr, Gamepad & gamepad)
{
    ostr << "{\n";

    ostr << "  " << gamepad.getAxisThreshold() << '\n';

    for (std::size_t i = 0; i < ACT_COUNT; ++i)
    {
        unsigned button = gamepad.getButtonFromAction((ActionId) i);
        if (button != sf::Joystick::ButtonCount)
        {
            ostr << "  " << i << ' ' << button << '\n';
        }
    }
    ostr << '}';

    return ostr;
}

std::istream & operator >> (std::istream & istr, Gamepad & gamepad)
{
    gamepad.reloadDefaults();

    istr >> std::ws;
    if (istr.peek() != '{')
    {
        IWBAN_LOG_ERROR("Error while reading gamepad configuration: { not found\n");
        return istr;
    }
    istr.get();

    float thresh;
    istr >> thresh;
    gamepad.setAxisThreshold(thresh);

    istr >> std::ws;
    while (istr.peek() != '}')
    {
        ActionId act;
        unsigned button;

        {
            unsigned a;
            istr >> a >> button;
            act = (ActionId) a;
        }

        if (!istr.good())
        {
            IWBAN_LOG_ERROR("Unexpected failure while reading gamepad configuration\n");
            return istr;
        }

        if (act >= ACT_COUNT || button >= sf::Joystick::ButtonCount)
        {
            IWBAN_LOG_ERROR("Invalid values for action and key while reading gamepad configuration\n");
            continue;
        }

        gamepad.mapButtonToAction(button, act);

        istr >> std::ws;
    }
    istr.get();

    return istr;
}

}
// namespace sys
