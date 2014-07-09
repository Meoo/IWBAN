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
    : _controls(controls)
{
    reloadDefaults();
}

void Gamepad::mapButtonToAction(Button button, ActionId action)
{
    IWBAN_PRE(button < sf::Joystick::ButtonCount);
    IWBAN_PRE(action < ACT_COUNT);

    // Ignore navigation actions
    if (action == ACT_ACCEPT || action == ACT_MENU)
    {
        IWBAN_LOG_WARNING("Trying to set a navigation action\n");
        return;
    }

    // For gamepad, we do not really care whatever happens to previous
    // configuration since we have fixed navigation keys
    unmapButton(button);
    unmapAction(action);

    _button_to_action[button] = action;
    _action_to_button[action] = button;
}

void Gamepad::unmapButton(Button button)
{
    IWBAN_PRE(button < sf::Joystick::ButtonCount);

    ActionId action = _button_to_action[button];

    if (action != ACT_COUNT)
    {
        _action_to_button[action] = sf::Joystick::ButtonCount;
        _button_to_action[button] = ACT_COUNT;
    }
}

void Gamepad::unmapAction(ActionId action)
{
    IWBAN_PRE(action < ACT_COUNT);

    Button button = _action_to_button[action];

    if (button != sf::Joystick::ButtonCount)
    {
        _action_to_button[action] = sf::Joystick::ButtonCount;
        _button_to_action[button] = ACT_COUNT;
    }
}

void Gamepad::reloadDefaults()
{
    // Clean first
    // We use ACT_COUNT as an invalid id
    for (std::size_t i = 0; i < sf::Joystick::ButtonCount; ++i)
        _button_to_action[i] = ACT_COUNT;

    // We use KeyCount as an invalid id
    for (std::size_t i = 0; i < ACT_COUNT; ++i)
        _action_to_button[i] = sf::Joystick::ButtonCount;

    // Initial configuration
    setAxisThreshold(IWBAN_GAMEPAD_AXIS_THRESH);

    mapButtonToAction(IWBAN_GAMEPAD_JUMP, ACT_JUMP);
    mapButtonToAction(IWBAN_GAMEPAD_FIRE, ACT_FIRE);
    mapButtonToAction(IWBAN_GAMEPAD_RETRY, ACT_RETRY);
}

unsigned Gamepad::getButtonFromAction(ActionId action)
{
    IWBAN_PRE(action < ACT_COUNT);

    return _action_to_button[action];
}

void Gamepad::onButtonPressed(unsigned button)
{
    IWBAN_PRE(button < sf::Joystick::ButtonCount);

    if (_catch_hook)
    {
        _catch_hook(button);
        _catch_hook = nullptr;
        return;
    }

    // Navigation configuration is fixed
    switch (button)
    {
    case IWBAN_GAMEPAD_ACCEPT:
        _controls.getAction(ACT_ACCEPT).activate();
        break;

    case IWBAN_GAMEPAD_MENU:
        _controls.getAction(ACT_MENU).activate();
        break;

    default:
        // Buttons can be used for both navigation and actions
        break;
    }

    // Dynamic configuration
    ActionId id = _button_to_action[button];
    if (id != ACT_COUNT)
        _controls.getAction(id).activate();
}

void Gamepad::onButtonReleased(unsigned button)
{
    IWBAN_PRE(button < sf::Joystick::ButtonCount);

    // Navigation configuration is fixed
    switch (button)
    {
    case IWBAN_GAMEPAD_ACCEPT:
        _controls.getAction(ACT_ACCEPT).deactivate();
        break;

    case IWBAN_GAMEPAD_MENU:
        _controls.getAction(ACT_MENU).deactivate();
        break;

    default:
        // Buttons can be used for both navigation and actions
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
        // No break

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
        Gamepad::Button button = gamepad.getButtonFromAction((ActionId) i);
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
        Gamepad::Button button;

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
            IWBAN_LOG_ERROR("Invalid values for action and key while reading"
                            " gamepad configuration\n");
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
