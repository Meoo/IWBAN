/**
 * @file   Settings.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SETTINGS_HPP_
#define _IWBAN_SETTINGS_HPP_

#include <Global.hpp>

namespace cfg
{

extern bool         fullscreen;
extern bool         threading;

extern float        zoom_multiplier;

extern unsigned     window_width;
extern unsigned     window_height;

extern unsigned     framerate;
extern bool         vsync;

extern std::string  language;

extern bool         pixelated;

extern bool         light_smooth;
extern unsigned     particles;

// ---- ---- ---- ----

void parseArgs(int argc, char ** argv);

void save();
void load();

}
// namespace cfg

#endif // _IWBAN_SETTINGS_HPP_
