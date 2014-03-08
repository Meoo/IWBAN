/**
 * @file   Config.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CONFIG_HPP_
#define _IWBAN_CONFIG_HPP_

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

extern bool         smooth_light;

// ---- ---- ---- ----

void parse_args(int argc, char ** argv);

void save();
void load();

}
// namespace cfg

#endif // _IWBAN_CONFIG_HPP_
