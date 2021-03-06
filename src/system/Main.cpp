/**
 * @file   Main.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/DisplayConfig.hpp>
#include <config/PathsConfig.hpp>
#include <config/Settings.hpp>

#include <data/Locale.hpp>

#include <game/core/InitialScreen.hpp>

#include <resources/Async.hpp>

#include <system/Display.hpp>
#include <system/Exception.hpp>
#include <system/Projector.hpp>
#include <system/exceptions/RestartApp.hpp>
#include <system/screens/ExceptionScreen.hpp>

int main(int argc, char ** argv)
{
    IWBAN_LOG_INFO("Version " IWBAN_VERSION " (" __USER__ ") - " __DATE__ " " __TIME__);

    IWBAN_LOG_INFO("Loading configuration");
    cfg::load();
    cfg::parseArgs(argc, argv);


    /*IWBAN_LOG_INFO("Initializing threads");
    res::async::initialize(cfg::threading);*/


#ifndef NDEBUG
    IWBAN_LOG_INFO("Starting std input polling");
    res::async::startStdInputPolling();
#endif

    IWBAN_LOG_INFO("Loading fonts");
    data::getLocale().loadFont(IWBAN_DEFAULT_FONT);


    IWBAN_LOG_INFO("Initializing projector");
    sys::Projector projector(game::createInitialScreen(argc, argv));


    IWBAN_LOG_INFO("Opening display");
    sys::Display display;
    display.open();


    // Loop until the game is closed
    IWBAN_LOG_INFO("Entering main loop");
    bool close = false;
    bool exception_caught = false;
    while (!close)
    {
        try
        {
            // Run the game (blocking)
            display.run(projector);

            // Exit the loop if the game is closed in a clean way
            close = true;
        }
        catch (const sys::RestartApp & exception)
        {
            IWBAN_LOG_INFO("Restarting application : %s", exception.what());

            // Reload the display
            display.close();
            display.open();
        }
        catch (const sys::Exception & exception)
        {
            IWBAN_LOG_CRITICAL("%s : %s", exception.title(), exception.what());

            if (exception_caught) throw;
            exception_caught = true;

            // IWBAN exception caught
            projector.setScreen(
                new sys::ExceptionScreen(exception.title(), exception.what()));
        }
        catch (const std::exception & exception)
        {
            IWBAN_LOG_CRITICAL("Exception : %s", exception.what());

            if (exception_caught) throw;
            exception_caught = true;

            // Standard exception caught
            projector.setScreen(
                new sys::ExceptionScreen("Error !", exception.what()));
        }
        catch (...)
        {
            IWBAN_LOG_CRITICAL("Unknown exception caught");

            if (exception_caught) throw;
            exception_caught = true;

            // Unknown exception caught
            projector.setScreen(
                new sys::ExceptionScreen("Error !", "Unknown exception caught"));
        }
    }

    IWBAN_LOG_INFO("Closing display");
    display.close();


    /*IWBAN_LOG_INFO("Terminating threads");
    res::async::terminate();*/


    IWBAN_LOG_INFO("Saving configuration");
    cfg::save();

    return 0;
}
