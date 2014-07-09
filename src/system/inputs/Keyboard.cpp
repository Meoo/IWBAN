/**
 * @file   Keyboard.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/KeyboardConfig.hpp>

#include <system/inputs/Keyboard.hpp>

namespace sys
{

Keyboard::Keyboard(Controls & controls)
    : _controls(controls)
{
    reloadDefaults();
}

void Keyboard::mapKeyToAction(sf::Keyboard::Key key, ActionId action)
{
    IWBAN_PRE_RANGE(key, 0, sf::Keyboard::KeyCount);
    IWBAN_PRE(action < ACT_COUNT);

    // Ignore navigation actions
    if (action == ACT_ACCEPT || action == ACT_MENU)
    {
        IWBAN_LOG_WARNING("Trying to set a navigation action\n");
        return;
    }

    // Ignore navigation keys
    switch (key)
    {
    case IWBAN_KEYBOARD_ACCEPT:
    case IWBAN_KEYBOARD_MENU:
    case IWBAN_KEYBOARD_UP:
    case IWBAN_KEYBOARD_DOWN:
    case IWBAN_KEYBOARD_LEFT:
    case IWBAN_KEYBOARD_RIGHT:
        IWBAN_LOG_WARNING("Trying to override a fixed navigation key\n");
        return;

    default:
        break;
    }

    // For keyboard, we do not really care whatever happens to previous
    // configuration since we have fixed navigation keys
    unmapKey(key);
    unmapAction(action);

    _key_to_action[key] = action;
    _action_to_key[action] = key;
}

void Keyboard::unmapKey(sf::Keyboard::Key key)
{
    IWBAN_PRE_RANGE(key, 0, sf::Keyboard::KeyCount);

    ActionId action = _key_to_action[key];

    if (action != ACT_COUNT)
    {
        _action_to_key[action] = sf::Keyboard::KeyCount;
        _key_to_action[key] = ACT_COUNT;
    }
}

void Keyboard::unmapAction(ActionId action)
{
    IWBAN_PRE(action < ACT_COUNT);

    sf::Keyboard::Key key = _action_to_key[action];

    if (key != sf::Keyboard::KeyCount)
    {
        _action_to_key[action] = sf::Keyboard::KeyCount;
        _key_to_action[key] = ACT_COUNT;
    }
}

void Keyboard::reloadDefaults()
{
    // Clean first
    // We use ACT_COUNT as an invalid id
    for (std::size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
        _key_to_action[i] = ACT_COUNT;

    // We use KeyCount as an invalid id
    for (std::size_t i = 0; i < ACT_COUNT; ++i)
        _action_to_key[i] = sf::Keyboard::KeyCount;

    // Default configuration
    mapKeyToAction(IWBAN_KEYBOARD_JUMP, ACT_JUMP);
    mapKeyToAction(IWBAN_KEYBOARD_FIRE, ACT_FIRE);
    mapKeyToAction(IWBAN_KEYBOARD_RETRY, ACT_RETRY);
}

sf::Keyboard::Key Keyboard::getKeyFromAction(ActionId action)
{
    IWBAN_PRE(action < ACT_COUNT);

    return _action_to_key[action];
}

void Keyboard::onKeyPressed(sf::Keyboard::Key key)
{
    IWBAN_PRE_RANGE(key, 0, sf::Keyboard::KeyCount);

    // Catch the first key pressed if asked for
    if (_catch_hook)
    {
        _catch_hook(key);
        _catch_hook = nullptr;
        return;
    }

    // Find the action ID from either static configuration
    // or key mappings
    ActionId id;
    switch (key)
    {
    case IWBAN_KEYBOARD_ACCEPT:
        id = ACT_ACCEPT;
        break;

    case IWBAN_KEYBOARD_MENU:
        id = ACT_MENU;
        break;

    case IWBAN_KEYBOARD_UP:
        id = ACT_UP;
        break;

    case IWBAN_KEYBOARD_DOWN:
        id = ACT_DOWN;
        break;

    case IWBAN_KEYBOARD_LEFT:
        id = ACT_LEFT;
        break;

    case IWBAN_KEYBOARD_RIGHT:
        id = ACT_RIGHT;
        break;

    default:
        id = _key_to_action[key];
        break;
    }

    // On keyboard, Jump = Accept and Fire = Cancel
    switch (id)
    {
    case ACT_JUMP:
        _controls.getAction(ACT_ACCEPT).activate();
        break;

    case ACT_COUNT:
        // Do nothing
        break;

    default:
        _controls.getAction(id).activate();
        break;
    }
}

void Keyboard::onKeyReleased(sf::Keyboard::Key key)
{
    IWBAN_PRE_RANGE(key, 0, sf::Keyboard::KeyCount);

    // Find the action ID from either static configuration
    // or key mappings
    ActionId id;
    switch (key)
    {
    case IWBAN_KEYBOARD_ACCEPT:
        id = ACT_ACCEPT;
        break;

    case IWBAN_KEYBOARD_MENU:
        id = ACT_MENU;
        break;

    case IWBAN_KEYBOARD_UP:
        id = ACT_UP;
        break;

    case IWBAN_KEYBOARD_DOWN:
        id = ACT_DOWN;
        break;

    case IWBAN_KEYBOARD_LEFT:
        id = ACT_LEFT;
        break;

    case IWBAN_KEYBOARD_RIGHT:
        id = ACT_RIGHT;
        break;

    default:
        id = _key_to_action[key];
        break;
    }

    // On keyboard, Jump = Accept and Fire = Cancel
    switch (id)
    {
    case ACT_JUMP:
        _controls.getAction(ACT_ACCEPT).deactivate();
        break;

    case ACT_COUNT:
        // Do nothing
        break;

    default:
        _controls.getAction(id).deactivate();
        break;
    }
}

// ---- ---- ---- ----

std::ostream & operator << (std::ostream & ostr, Keyboard & keyboard)
{
    ostr << "{\n";
    for (std::size_t i = 0; i < ACT_COUNT; ++i)
    {
        sf::Keyboard::Key key = keyboard.getKeyFromAction((ActionId) i);
        if (key != sf::Keyboard::KeyCount)
        {
            ostr << "  " << i << ' ' << key << '\n';
        }
    }
    ostr << '}';

    return ostr;
}

std::istream & operator >> (std::istream & istr, Keyboard & keyboard)
{
    keyboard.reloadDefaults();

    istr >> std::ws;
    if (istr.peek() != '{')
    {
        IWBAN_LOG_ERROR("Error while reading keyboard configuration: { not found\n");
        return istr;
    }
    istr.get();

    istr >> std::ws;
    while (istr.peek() != '}')
    {
        ActionId act;
        sf::Keyboard::Key key;

        {
            unsigned a, b;
            istr >> a >> b;
            act = (ActionId) a;
            key = (sf::Keyboard::Key) b;
        }

        if (!istr.good())
        {
            IWBAN_LOG_ERROR("Unexpected failure while reading keyboard configuration\n");
            return istr;
        }

        if (act >= ACT_COUNT || key >= sf::Keyboard::KeyCount)
        {
            IWBAN_LOG_ERROR("Invalid values for action and key while reading keyboard configuration\n");
            continue;
        }

        keyboard.mapKeyToAction(key, act);

        istr >> std::ws;
    }
    istr.get();

    return istr;
}

}
// namespace sys
