/**
 * @file   Keyboard.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <system/inputs/Keyboard.hpp>

namespace sys
{

Keyboard::Keyboard(Controls & controls)
    : _controls(controls)
{
    // We use ACT_COUNT as an invalid id
    for (std::size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
        _key_to_action[i] = ACT_COUNT;

    _key_to_action[sf::Keyboard::Return]    = ACT_JUMP;

    _key_to_action[sf::Keyboard::Up]        = ACT_UP;
    _key_to_action[sf::Keyboard::Down]      = ACT_DOWN;
    _key_to_action[sf::Keyboard::Left]      = ACT_LEFT;
    _key_to_action[sf::Keyboard::Right]     = ACT_RIGHT;

    _key_to_action[sf::Keyboard::Escape]    = ACT_MENU;
}

void Keyboard::onKeyPressed(sf::Keyboard::Key key)
{
    ActionId id = _key_to_action[key];

    if (id != ACT_COUNT)
        _controls.getAction(id).activate();
}

void Keyboard::onKeyReleased(sf::Keyboard::Key key)
{
    ActionId id = _key_to_action[key];

    if (id != ACT_COUNT)
        _controls.getAction(id).deactivate();
}

}
// namespace sys
