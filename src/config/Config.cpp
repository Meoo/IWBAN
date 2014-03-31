/**
 * @file   Config.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/Config.hpp>

#include <cctype>   // isalpha()
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

bool        light_smooth    = true;
bool        light_quality   = true;
unsigned    particles       = 2;

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
    SAVE(light_smooth);
    SAVE(light_quality);
    SAVE(particles);

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
        LOAD(light_smooth);
        LOAD(light_quality);
        LOAD(particles);

        f >> std::ws;
    }

    f.close();

    // Check values

    if (language.size() != 2
        || !std::isalpha(language[0])
        || !std::isalpha(language[1]))
    {
        IWBAN_LOG_WARNING("Invalid value for 'language', setting to default\n");
        language = "fr";
    }

    if (particles > 2)
    {
        IWBAN_LOG_WARNING("Invalid value for 'particles', setting to default\n");
        particles = 2;
    }
}

#undef LOAD

}
// namespace cfg
