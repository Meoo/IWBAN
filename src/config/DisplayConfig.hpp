/**
 * @file   DisplayConfig.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_DISPLAYCONFIG_HPP_
#define _IWBAN_DISPLAYCONFIG_HPP_

#include <SFML/System.hpp>


#define IWBAN_GAME_NAME         "I Wanna Be The Ninja Master"
#define IWBAN_VERSION           "D.1"

#define IWBAN_FRAME_WIDTH       640 // px
#define IWBAN_FRAME_HEIGHT      480 // px

#define IWBAN_FRAME_RATE        60 // Hz

#define IWBAN_UPDATE_RATE       60 // Hz
#define IWBAN_UPDATE_TIME       sf::seconds(1.f/IWBAN_UPDATE_RATE)

// Maximum number of updates in a single frame
// If framerate goes under _UPDATE_RATE / _MAX_UPDATES_FRAME, the game will slow down
#define IWBAN_MAX_UPDATES_FRAME 3


#define IWBAN_SPLASH_DURATION   sf::seconds(4.f)


#endif // _IWBAN_DISPLAYCONFIG_HPP_
