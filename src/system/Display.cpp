/**
 * @file   Display.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/Settings.hpp>

#include <system/Controls.hpp>
#include <system/Display.hpp>
#include <system/Projector.hpp>
#include <system/Time.hpp>
#include <system/exceptions/RestartApp.hpp>
#include <system/inputs/Gamepad.hpp>
#include <system/inputs/Keyboard.hpp>

#include <resources/File.hpp>

// Manually enable perf. monitoring
#define PERF_MONITORING

// Always enabled in Debug mode
#ifndef NDEBUG
#  define PERF_MONITORING
#endif

#ifdef PERF_MONITORING
#  include <iostream>
#  include <iomanip> // setprecision
#endif

namespace sys
{

Display::Display()
    : bg_mesh(sf::Quads, 4)
{
    IWBAN_DEBUG(_ready = false);
}

// ---- ---- ---- ----

void Display::open()
{
    BOOST_ASSERT(!_ready);

    if (cfg::fullscreen == 1)
        // Fullscreen
        _window.create(sf::VideoMode::getFullscreenModes().at(0),
                IWBAN_GAME_NAME, sf::Style::Fullscreen);

    else if (cfg::fullscreen == 2)
        // Borderless fullscreen
        _window.create(sf::VideoMode::getFullscreenModes().at(0),
                IWBAN_GAME_NAME, sf::Style::None);

    else
        // Windowed
        _window.create(sf::VideoMode(cfg::window_width, cfg::window_height),
                IWBAN_GAME_NAME, sf::Style::Titlebar | sf::Style::Close);

    // Window properties
    _window.setFramerateLimit(cfg::framerate);
    _window.setVerticalSyncEnabled(cfg::vsync);
    _window.setMouseCursorVisible(false);
    _window.setKeyRepeatEnabled(false);

    // Window icon
    if (win_icon.getSize().x <= 0)
    {
        res::File icon_file = res::openFile("system/icon.png");
        if (!win_icon.loadFromMemory(icon_file.getData(), icon_file.getSize()))
            IWBAN_LOG_ERROR("Unable to load display's icon\n");
    }
    _window.setIcon(win_icon.getSize().x, win_icon.getSize().y,
                win_icon.getPixelsPtr());

    // Background data
    if (bg_tex.getSize().x <= 0)
    {
        res::File bg_file = res::openFile("system/background.png");
        if (bg_tex.loadFromMemory(bg_file.getData(), bg_file.getSize()))
        {
            bg_tex.setSmooth(true);
            bg_tex.setRepeated(true);
        }
        else
            IWBAN_LOG_ERROR("Unable to load display's background texture\n");

        bg_mesh[0].position = sf::Vector2f(0, 0);
        bg_mesh[1].position = sf::Vector2f(1, 0);
        bg_mesh[2].position = sf::Vector2f(1, 1);
        bg_mesh[3].position = sf::Vector2f(0, 1);

        bg_view.setCenter(0.5f, 0.5f);
        bg_view.setSize(1, 1);
    }

    IWBAN_DEBUG(_ready = true);
}
// Display::open()

// ---- ---- ---- ----

void Display::close()
{
    BOOST_ASSERT(_ready);

    _window.close();

    IWBAN_DEBUG(_ready = false);
}

// ---- ---- ---- ----

void Display::run(sys::Projector & projector)
{
    BOOST_ASSERT(_ready);

    updateSceneView();

    // Scene data
    gfx::Renderer   renderer(_window);
    sf::Time        next_update(sf::seconds(IWBAN_UPDATE_TIME));

#ifdef PERF_MONITORING
    // FPS counter
    sf::Clock       fps_clock;
    unsigned        fps_counter = 0;
    unsigned        total_fps_counter = 0;

    // Performance counters
    sf::Time        perf_event;
    sf::Time        perf_update;
    sf::Time        perf_draw;
    sf::Time        perf_display;

#   define PERF_BEGIN(type) perf_ ## type -= getGlobalTime()
#   define PERF_END(type)   perf_ ## type += getGlobalTime()

#else
#   define PERF_BEGIN(type)
#   define PERF_END(type)
#endif

#ifndef NDEBUG
    // Debbuging variables
    bool            pause = false;
    bool            play_next_frame = false;
#endif

    // ---- ---- ---- ----

    // Main loop
    while(_window.isOpen())
    {
        PERF_BEGIN(event);

        // Event polling
        sf::Event event;
        while (_window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                _window.close();
                break;

            case sf::Event::Resized:
                IWBAN_LOG_DEBUG("Window resized\n");
                if (_window.getSize().x < IWBAN_FRAME_WIDTH ||
                    _window.getSize().y < IWBAN_FRAME_HEIGHT)
                {
                    _window.setSize(sf::Vector2u(
                        std::max(_window.getSize().x, (unsigned) IWBAN_FRAME_WIDTH),
                        std::max(_window.getSize().y, (unsigned) IWBAN_FRAME_HEIGHT)));
                    break;
                }

                updateSceneView();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Unknown) continue;

                if (event.key.code == sf::Keyboard::Return
                        && event.key.alt && !event.key.shift
                        && !event.key.control && !event.key.system)
                {
                    // Alt + Enter : Toggle fullscreen
                    cfg::fullscreen = !cfg::fullscreen;
                    throw RestartApp("Alt + Enter pressed");
                }
#ifndef NDEBUG
                if (event.key.code == sf::Keyboard::Insert)
                {
                    pause = !pause;
                    play_next_frame = false;
                    IWBAN_LOG_DEBUG("Pause %s\n", pause ? "enabled" : "disabled");
                }
                if (event.key.code == sf::Keyboard::PageDown)
                    play_next_frame = true;
#endif
                getKeyboard().onKeyPressed(event.key.code);
                break;

            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Unknown) continue;

                getKeyboard().onKeyReleased(event.key.code);
                break;

            case sf::Event::JoystickButtonPressed:
                if (event.joystickButton.joystickId == 0)
                    getGamepad().onButtonPressed(event.joystickButton.button);
                break;

            case sf::Event::JoystickButtonReleased:
                if (event.joystickButton.joystickId == 0)
                    getGamepad().onButtonReleased(event.joystickButton.button);
                break;

            case sf::Event::JoystickMoved:
                if (event.joystickButton.joystickId == 0)
                    getGamepad().onAxisMoved(event.joystickMove.axis, event.joystickMove.position);
                break;

            case sf::Event::GainedFocus:
                IWBAN_LOG_DEBUG("Focus gained\n");
                break;

            case sf::Event::LostFocus:
                IWBAN_LOG_DEBUG("Focus lost\n");
                getControls().reset();
                break;

            default:
                break;
            }
        }

        PERF_END(event);
        PERF_BEGIN(update);

#ifndef NDEBUG
        // DEBUG : Allow the game to be paused by pressing Insert key
        if (pause)
        {
            sf::Time update_time = getGlobalTime();

            // Press PageDown key to process a single step
            if (play_next_frame)
            {
                IWBAN_LOG_DEBUG("Processing single frame\n");
                projector.update(update_time);
                play_next_frame = false;
            }

            // Prevent "Game is slowing down" warning on resume
            next_update = update_time + sf::seconds(IWBAN_UPDATE_TIME);

        } // if (pause)
        else
        {
#endif
            // Inflict a penalty on any update after the first one to increase consistency
            int update_count = 0;
            sf::Time update_time;
            while ((update_time = getGlobalTime()) > (update_count == 0 ?
                    next_update : next_update + sf::seconds(IWBAN_UPDATE_TIME / 4)))
            {
                // Scene update
                projector.update(update_time);

                next_update += sf::seconds(IWBAN_UPDATE_TIME);

                // Prevent game lock by limiting consecutive updates
                if (++update_count >= IWBAN_MAX_UPDATES_FRAME)
                {
                    // Issue a warning when the game is slowing down,
                    // and reset next_update timer to prevent
                    // update burst after a lag spike
                    sf::Time time (getGlobalTime());
                    if (time > next_update)
                    {
                        next_update = time + sf::seconds(IWBAN_UPDATE_TIME);
                        IWBAN_LOG_WARNING("Game is slowing down!\n");
                    }

                    break;
                }
            }
#ifndef NDEBUG
        } // else if (pause)
#endif

        PERF_END(update);
        PERF_BEGIN(draw);

        sf::Time render_time = getGlobalTime();

        // Background rendering
        if (marginX > 0 || marginY > 0)
        {
            _window.setView(bg_view);

            float t = render_time.asSeconds();
            float t2 = - t/2;

            bg_mesh[0].texCoords.x = t;
            bg_mesh[1].texCoords.x = winW2 + t;
            bg_mesh[2].texCoords.x = winW2 + t;
            bg_mesh[3].texCoords.x = t;

            bg_mesh[0].texCoords.y = t2;
            bg_mesh[1].texCoords.y = t2;
            bg_mesh[2].texCoords.y = winH2 + t2;
            bg_mesh[3].texCoords.y = winH2 + t2;

            _window.draw(bg_mesh, &bg_tex);

            _window.setView(render_view);
        }

        // Scene rendering
        renderer.begin();
        projector.render(renderer, render_time);
        renderer.end();

        PERF_END(draw);
        PERF_BEGIN(display);

        // Display on screen
        _window.display();

        PERF_END(display);

#ifdef PERF_MONITORING
        // Display FPS results every 1000 frames
        if ((++fps_counter) >= 1000)
        {
            std::cout << "FPS  : " << (fps_counter /
                    fps_clock.getElapsedTime().asSeconds()) << std::endl;
            fps_clock.restart();
            fps_counter = 0;
        }
        ++total_fps_counter;
#endif

    } // while(win.isOpen())

#ifdef PERF_MONITORING
    sf::Time perf_total = getGlobalTime();

    // Dump performance monitoring results to standard output
    std::cout << "Average FPS : " << (total_fps_counter / perf_total.asSeconds()) << std::endl
              << "  Time elapsed in the different sections (in %) :" << std::endl
              << std::setprecision(1) << std::fixed
              << "  Events  : " << (perf_event / perf_total) * 100 << std::endl
              << "  Update  : " << (perf_update / perf_total) * 100 << std::endl
              << "  Draw    : " << (perf_draw / perf_total) * 100 << std::endl
              << "  Display : " << (perf_display / perf_total) * 100 << std::endl
              << "  Other   : " << (1 - (perf_event + perf_update
                                       + perf_draw + perf_display)
                                           / perf_total) * 100 << std::endl;
#endif
}
// Display::run()

// ---- ---- ---- ----

void Display::updateSceneView()
{
    zoom = std::min(_window.getSize().x / (float) IWBAN_FRAME_WIDTH,
                    _window.getSize().y / (float) IWBAN_FRAME_HEIGHT);

    if (cfg::zoom_multiplier)
        zoom = zoom - std::fmod(zoom + 0.00001f, cfg::zoom_multiplier);

    winW2 = _window.getSize().x / 2;
    winH2 = _window.getSize().y / 2;
    sceneW = IWBAN_FRAME_WIDTH * zoom;
    sceneH = IWBAN_FRAME_HEIGHT * zoom;
    marginX = (_window.getSize().x - sceneW) / 2;
    marginY = (_window.getSize().y - sceneH) / 2;
    render_view.setCenter(IWBAN_FRAME_WIDTH / 2, IWBAN_FRAME_HEIGHT / 2);
    render_view.setSize(IWBAN_FRAME_WIDTH + marginX * 2 / zoom,
                        IWBAN_FRAME_HEIGHT + marginY * 2 / zoom);

    _window.setView(render_view);
}

}
// namespace sys
