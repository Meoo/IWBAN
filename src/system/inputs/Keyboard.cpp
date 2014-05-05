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
    // We use ACT_COUNT as an invalid id
    for (std::size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
        _key_to_action[i] = ACT_COUNT;

    // We use KeyCount as an invalid id
    for (std::size_t i = 0; i < ACT_COUNT; ++i)
        _action_bindings[i] = sf::Keyboard::KeyCount;

    // Static configuration : Cannot be changed
    _key_to_action[IWBAN_KEYBOARD_JUMP]     = ACT_JUMP;
    _key_to_action[IWBAN_KEYBOARD_FIRE]     = ACT_FIRE;
    _key_to_action[IWBAN_KEYBOARD_MENU]     = ACT_MENU;
    _key_to_action[IWBAN_KEYBOARD_RETRY]    = ACT_RETRY;

    _key_to_action[IWBAN_KEYBOARD_UP]       = ACT_UP;
    _key_to_action[IWBAN_KEYBOARD_DOWN]     = ACT_DOWN;
    _key_to_action[IWBAN_KEYBOARD_LEFT]     = ACT_LEFT;
    _key_to_action[IWBAN_KEYBOARD_RIGHT]    = ACT_RIGHT;
}

void Keyboard::mapKeyToAction(sf::Keyboard::Key key, ActionId action)
{
    BOOST_ASSERT(key < sf::Keyboard::KeyCount && action < ACT_COUNT);

    if (_key_to_action[key] != ACT_COUNT)
        IWBAN_LOG_WARNING("Trying to map a key which is already bound\n");

    else
    {
        _key_to_action[key] = action;
        _action_bindings[action] = key;
    }
}

void Keyboard::unmapKey(sf::Keyboard::Key key)
{
    BOOST_ASSERT(key < sf::Keyboard::KeyCount);

    // Remove from action mapping if required
    ActionId act = _key_to_action[key];

    if (_action_bindings[act] == key)
        _action_bindings[act] = sf::Keyboard::KeyCount;

    // Remove from key mapping
    _key_to_action[key] = ACT_COUNT;
}

sf::Keyboard::Key Keyboard::getKeyFromAction(ActionId action)
{
    BOOST_ASSERT(action < ACT_COUNT);

    return _action_bindings[action];
}

void Keyboard::onKeyPressed(sf::Keyboard::Key key)
{
    BOOST_ASSERT(key < sf::Keyboard::KeyCount);

    ActionId id = _key_to_action[key];

    if (id != ACT_COUNT)
        _controls.getAction(id).activate();
}

void Keyboard::onKeyReleased(sf::Keyboard::Key key)
{
    BOOST_ASSERT(key < sf::Keyboard::KeyCount);

    ActionId id = _key_to_action[key];

    if (id != ACT_COUNT)
        _controls.getAction(id).deactivate();
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
