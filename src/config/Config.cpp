/**
 * @file   Config.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/Config.hpp>

#include <fstream>

namespace cfg
{

bool        fullscreen      = false;
bool        threading       = true;

float       zoom_multiplier = 0.f;

unsigned    window_width    = IWBAN_FRAME_WIDTH;
unsigned    window_height   = IWBAN_FRAME_HEIGHT;

unsigned    framerate       = IWBAN_FRAMERATE;
bool        vsync           = false;

std::string language        = "fr";

bool        smooth_light    = true;

// ---- ---- ---- ----

void parse_args(int argc, char ** argv)
{
}

#define SAVE(var) f << #var << " " << var << "\n"

void save()
{
    std::ofstream f(IWBAN_CONFIG_FILE);
    if (!f.is_open())
    {
        IWBAN_LOG_WARNING("Unable to save configuration : " IWBAN_CONFIG_FILE "\n");
        return;
    }

    SAVE(fullscreen);
    SAVE(threading);
    SAVE(zoom_multiplier);
    SAVE(window_width);
    SAVE(window_height);
    SAVE(framerate);
    SAVE(vsync);
    SAVE(language);
    SAVE(smooth_light);

    f.flush();
    f.close();
}

#undef SAVE

#define LOAD(var) if (!n.compare(#var)) f >> var

void load()
{
    std::ifstream f(IWBAN_CONFIG_FILE);
    if (!f.is_open())
    {
        IWBAN_LOG_WARNING("Unable to load configuration : " IWBAN_CONFIG_FILE "\n");
        return;
    }

    while (!f.eof())
    {
        std::string n;
        f >> n;

        LOAD(fullscreen);
        LOAD(threading);
        LOAD(zoom_multiplier);
        LOAD(window_width);
        LOAD(window_height);
        LOAD(framerate);
        LOAD(vsync);
        LOAD(language);
        LOAD(smooth_light);

        f >> std::ws;
    }

    f.close();
}

#undef LOAD

}
// namespace cfg
