/**
 * @file   Settings.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/Settings.hpp>

#include <system/inputs/Gamepad.hpp>
#include <system/inputs/Keyboard.hpp>

#include <cctype>   // isalpha()
#include <fstream>

namespace cfg
{

unsigned    fullscreen      = 0;
bool        threading       = true;

float       zoom_multiplier = 0.f;

unsigned    window_width    = IWBAN_FRAME_WIDTH;
unsigned    window_height   = IWBAN_FRAME_HEIGHT;

unsigned    framerate       = IWBAN_FRAME_RATE;
bool        vsync           = false;

unsigned    volume          = 50;

std::string language        = "fr";

bool        pixelated       = false;

unsigned    particles       = 2;

// ---- ---- ---- ----

void parseArgs(int argc, char ** argv)
{
}

#define SAVE(var) f << #var << " " << var << "\n"

void save()
{
    // Get keyboard and gamepad reference for SAVE macro
    sys::Keyboard & keyboard = sys::getKeyboard();
    sys::Gamepad & gamepad = sys::getGamepad();

    std::ofstream f(IWBAN_CONFIG_FILE);
    if (!f.is_open())
    {
        IWBAN_LOG_ERROR("Unable to save configuration : " IWBAN_CONFIG_FILE);
        return;
    }

    SAVE(fullscreen);
    SAVE(threading);
    SAVE(zoom_multiplier);
    SAVE(window_width);
    SAVE(window_height);
    SAVE(framerate);
    SAVE(vsync);
    SAVE(volume);
    SAVE(language);
    SAVE(pixelated);
    SAVE(particles);
    SAVE(keyboard);
    SAVE(gamepad);

    f.flush();
    if (!f.good())
    {
        IWBAN_LOG_ERROR("An error occured while saving configuration");
    }
    f.close();
}

#undef SAVE

#define LOAD(var) else if (!n.compare(#var)) f >> var

void load()
{
    // Get keyboard and gamepad reference for LOAD macro
    sys::Keyboard & keyboard = sys::getKeyboard();
    sys::Gamepad & gamepad = sys::getGamepad();

    std::ifstream f(IWBAN_CONFIG_FILE);
    if (!f.is_open())
    {
        IWBAN_LOG_WARNING("Unable to load configuration : " IWBAN_CONFIG_FILE);
        return;
    }

    while (!f.eof())
    {
        std::string n;
        f >> n >> std::ws;

        if (!f.good()) // eof is an error
        {
            IWBAN_LOG_ERROR("Unexpected error after key : '%s'", n.c_str());
            return;
        }

        // Start of loading zone which use "else if"
        if (false) {}

        LOAD(fullscreen);
        LOAD(threading);
        LOAD(zoom_multiplier);
        LOAD(window_width);
        LOAD(window_height);
        LOAD(framerate);
        LOAD(vsync);
        LOAD(volume);
        LOAD(language);
        LOAD(pixelated);
        LOAD(particles);
        LOAD(keyboard);
        LOAD(gamepad);

        else
            IWBAN_LOG_ERROR("Invalid key : '%s'", n.c_str());

        if (f.bad() || f.fail()) // eof is not an error
        {
            IWBAN_LOG_ERROR("Unexpected error while reading value for key"
                            " : '%s'", n.c_str());
            return;
        }

        f >> std::ws;
    }
    f.close();


    // Check values

    if (fullscreen > 2)
    {
        IWBAN_LOG_WARNING("Invalid value for 'fullscreen', setting to default");
        particles = 0;
    }

    if (volume > 100)
    {
        IWBAN_LOG_WARNING("Invalid value for 'volume', setting to default");
        volume = 100;
    }

    if (language.size() != 2
        || !std::isalpha(language[0])
        || !std::isalpha(language[1]))
    {
        IWBAN_LOG_WARNING("Invalid value for 'language', setting to default");
        language = "fr";
    }

    if (particles > 2)
    {
        IWBAN_LOG_WARNING("Invalid value for 'particles', setting to default");
        particles = 2;
    }
}

#undef LOAD

}
// namespace cfg
