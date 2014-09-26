/**
 * @file   InitialScreen.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <game/core/InitialScreen.hpp>

#include <system/screens/MainMenuScreen.hpp>
#include <system/screens/SplashScreen.hpp>

namespace game
{

sys::Screen * createInitialScreen(int argc, char ** argv)
{
    sys::Screen * screen = new sys::MainMenuScreen();
    screen = new sys::SplashScreen(screen);

    // TODO Shader warning screen
    /*if (sf::Shader::isAvailable())
        screen = new sys::ShaderWarningScreen(screen);*/

    return screen;
}

}
// namespace game
