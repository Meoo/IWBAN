/**
 * @file   StaticConfig.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_STATICCONFIG_HPP_
#define _IWBAN_STATICCONFIG_HPP_


#define IWBAN_GAME_NAME         "I Wanna Be The Ninja Master"

#define IWBAN_FRAME_WIDTH       640 // px
#define IWBAN_FRAME_HEIGHT      480 // px

#define IWBAN_FRAME_RATE        60 // Hz


#define IWBAN_DATA_FOLDER       "data"
#define IWBAN_USER_FOLDER       "."

#define IWBAN_CONFIG_FILE       IWBAN_USER_FOLDER "/iwban.cfg"

#define IWBAN_DEFAULT_FONT      "system/poetsen_one.ttf"


// Maximum capacity : 4096 entities
#define IWBAN_ENTS_BLK_SIZE     512
#define IWBAN_ENTS_BLK_COUNT    16
#define IWBAN_MAX_ENTITIES      (IWBAN_ENTS_BLK_SIZE * IWBAN_ENTS_BLK_COUNT)


#define IWBAN_UPDATE_RATE       60 // Hz
#define IWBAN_UPDATE_TIME       (1.f/IWBAN_UPDATE_RATE) // s

// Maximum number of updates in a single frame
// If framerate goes under _UPDATE_RATE / _MAX_UPDATES_FRAME, the game will slow down
#define IWBAN_MAX_UPDATES_FRAME 3


#define IWBAN_SPLASH_DURATION   4 // s


#endif // _IWBAN_STATICCONFIG_HPP_
