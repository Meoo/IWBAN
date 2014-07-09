/**
 * @file   KeyboardConfig.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_KEYBOARDCONFIG_HPP_
#define _IWBAN_KEYBOARDCONFIG_HPP_


#include <SFML/Window.hpp>

// Default configuration
#define IWBAN_KEYBOARD_JUMP     sf::Keyboard::X
#define IWBAN_KEYBOARD_FIRE     sf::Keyboard::C
#define IWBAN_KEYBOARD_RETRY    sf::Keyboard::R

// Fixed configuration
#define IWBAN_KEYBOARD_ACCEPT   sf::Keyboard::Return
#define IWBAN_KEYBOARD_MENU     sf::Keyboard::Escape

#define IWBAN_KEYBOARD_UP       sf::Keyboard::Up
#define IWBAN_KEYBOARD_DOWN     sf::Keyboard::Down
#define IWBAN_KEYBOARD_LEFT     sf::Keyboard::Left
#define IWBAN_KEYBOARD_RIGHT    sf::Keyboard::Right


#endif // _IWBAN_KEYBOARDCONFIG_HPP_
