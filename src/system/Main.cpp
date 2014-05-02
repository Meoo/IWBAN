/**
 * @file   Main.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/Config.hpp>

//#include <resources/Async.hpp>

#include <system/Display.hpp>
#include <system/Exception.hpp>
#include <system/Projector.hpp>
#include <system/exceptions/RestartApp.hpp>
#include <system/screens/ExceptionScreen.hpp>
#include <system/screens/IntroScreen.hpp>

int main(int argc, char ** argv)
{
    IWBAN_LOG_INFO("Loading configuration\n");
    cfg::load();
    cfg::parse_args(argc, argv);


    /*IWBAN_LOG_INFO("Initializing threads\n");
    res::async::initialize(cfg::threading);*/


    IWBAN_LOG_INFO("Opening display\n");
    sys::Display display;
    display.open();


    IWBAN_LOG_INFO("Initializing projector\n");
    sys::Projector projector(new sys::IntroScreen());


    // Loop until the game is closed
    IWBAN_LOG_INFO("Entering main loop\n");
    bool close = false;
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
            IWBAN_LOG_INFO("Restarting application : %s\n", exception.what());

            // Reload the display
            display.close();
            display.open();
        }
        catch (const sys::Exception & exception)
        {
            IWBAN_LOG_ERROR("%s : %s\n", exception.title(), exception.what());

            // IWBAN exception caught
            projector.setScreen(
                new sys::ExceptionScreen(projector.getCurrentScreen()));
        }
        catch (const std::exception & exception)
        {
            IWBAN_LOG_ERROR("Exception : %s\n", exception.what());

            // Standard exception caught
            projector.setScreen(
                new sys::ExceptionScreen(projector.getCurrentScreen()));
        }
        catch (...)
        {
            IWBAN_LOG_ERROR("Unknown exception caught\n");

            // Unknown exception caught
            projector.setScreen(
                new sys::ExceptionScreen(projector.getCurrentScreen()));
        }
    }

    IWBAN_LOG_INFO("Closing display\n");
    display.close();


    /*IWBAN_LOG_INFO("Terminating threads\n");
    res::async::terminate();*/


    IWBAN_LOG_INFO("Saving configuration\n");
    cfg::save();

    return 0;
}
