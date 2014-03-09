/**
 * @file   Display.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/Config.hpp>

#include <system/Display.hpp>
#include <system/exceptions/RestartApp.hpp>

#include <graphics/Renderer.hpp>

#include <resources/File.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#ifndef NDEBUG
#include <iomanip> // setprecision
#endif

namespace
{

IWBAN_DEBUG(bool ready = false);

// Render window
sf::RenderWindow    win;

// Icon and background are fixed
sf::Image           win_icon;
sf::Texture         bg_tex;
sf::VertexArray     bg_mesh(sf::Quads, 4);
sf::View            bg_view;

// Screen size and view data
float               zoom;
int                 sceneW;
int                 sceneH;
int                 marginX;
int                 marginY;
float               winW2;
float               winH2;
sf::View            render_view;

void update_scene_view()
{
    zoom = std::min(win.getSize().x / (float) IWBAN_FRAME_WIDTH,
                    win.getSize().y / (float) IWBAN_FRAME_HEIGHT);
    if (cfg::zoom_multiplier)
        zoom = zoom - std::fmod(zoom + 0.00001f, cfg::zoom_multiplier);
    winW2 = win.getSize().x / 2;
    winH2 = win.getSize().y / 2;
    sceneW = IWBAN_FRAME_WIDTH * zoom;
    sceneH = IWBAN_FRAME_HEIGHT * zoom;
    marginX = (win.getSize().x - sceneW) / 2;
    marginY = (win.getSize().y - sceneH) / 2;
    render_view.setCenter(IWBAN_FRAME_WIDTH / 2, IWBAN_FRAME_HEIGHT / 2);
    render_view.setSize(IWBAN_FRAME_WIDTH + marginX * 2 / zoom,
                        IWBAN_FRAME_HEIGHT + marginY * 2 / zoom);
}

}
// namespace

// ---- ---- ---- ----

namespace sys
{

void display_open()
{
    BOOST_ASSERT(!ready);

    if (cfg::fullscreen)
        // Fullscreen
        win.create(sf::VideoMode::getFullscreenModes().at(0),
                   IWBAN_GAME_NAME, sf::Style::Fullscreen);

    else
        // Windowed
        win.create(sf::VideoMode(cfg::window_width, cfg::window_height),
                   IWBAN_GAME_NAME, sf::Style::Default);

    // Window properties
    win.setFramerateLimit(cfg::framerate);
    win.setVerticalSyncEnabled(cfg::vsync);
    win.setMouseCursorVisible(false);
    win.setKeyRepeatEnabled(false);

    // Window icon
    if (win_icon.getSize().x <= 0)
    {
        res::File icon_file = res::openFile("system/icon.png");
        win_icon.loadFromMemory(icon_file.getData(), icon_file.getSize());
    }
    win.setIcon(win_icon.getSize().x, win_icon.getSize().y,
                win_icon.getPixelsPtr());

    // Background data
    if (bg_tex.getSize().x <= 0)
    {
        res::File bg_file = res::openFile("system/background.png");
        bg_tex.loadFromMemory(bg_file.getData(), bg_file.getSize());
        bg_tex.setSmooth(true);
        bg_tex.setRepeated(true);

        bg_mesh[0].position = sf::Vector2f(0, 0);
        bg_mesh[1].position = sf::Vector2f(1, 0);
        bg_mesh[2].position = sf::Vector2f(1, 1);
        bg_mesh[3].position = sf::Vector2f(0, 1);

        bg_view.setCenter(0.5f, 0.5f);
        bg_view.setSize(1, 1);
    }

    IWBAN_DEBUG(ready = true);
}

// ---- ---- ---- ----

void display_close()
{
    BOOST_ASSERT(ready);

    win.close();

    IWBAN_DEBUG(ready = false);
}

// ---- ---- ---- ----

void display_run(sys::ScreenProjector & projector)
{
    BOOST_ASSERT(ready);

    ::update_scene_view();

    // Scene data
    gfx::Renderer   renderer(win);
    sf::Clock       global_clock;
    sf::Time        next_update(sf::seconds(IWBAN_UPDATE_TIME));

#ifndef NDEBUG
    // FPS counter
    sf::Clock       fps_clock;
    unsigned        fps_counter = 0;
    unsigned        total_fps_counter = 0;

    // Performance counters
    sf::Time        perf_event;
    sf::Time        perf_update;
    sf::Time        perf_draw;
    sf::Time        perf_display;
#endif

    // ---- ---- ---- ----

    // Main loop
    while(win.isOpen())
    {
#ifndef NDEBUG
        perf_event -= global_clock.getElapsedTime();
#endif

        // Event polling
        sf::Event event;
        while (win.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                win.close();
                break;

            case sf::Event::Resized:
                IWBAN_LOG_DEBUG("Window resized\n");
                if (win.getSize().x < IWBAN_FRAME_WIDTH ||
                    win.getSize().y < IWBAN_FRAME_HEIGHT)
                {
                    win.setSize(sf::Vector2u(
                        std::max(win.getSize().x, (unsigned) IWBAN_FRAME_WIDTH),
                        std::max(win.getSize().y, (unsigned) IWBAN_FRAME_HEIGHT)));
                    break;
                }

                ::update_scene_view();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Return
                        && event.key.alt && !event.key.shift
                        && !event.key.control && !event.key.system)
                {
                    // Alt + Enter : Toggle fullscreen
                    cfg::fullscreen = !cfg::fullscreen;
                    throw RestartApp("Alt + Enter pressed");
                }
                break;

            case sf::Event::GainedFocus:
                IWBAN_LOG_DEBUG("Focus gained\n");
                break;

            case sf::Event::LostFocus:
                IWBAN_LOG_DEBUG("Focus lost\n");
                break;

            default:
                break;
            }
        }

#ifndef NDEBUG
        perf_event += global_clock.getElapsedTime();
        perf_update -= global_clock.getElapsedTime();
#endif

        // Update rate is always IWBAN_UPDATE_RATE
        int update_count = 0;
        while (global_clock.getElapsedTime() > next_update)
        {
            // Scene update
            projector.update();

            next_update += sf::seconds(IWBAN_UPDATE_TIME);

            // Prevent game lock by limiting consecutive updates
            if (++update_count >= IWBAN_MAX_UPDATES_FRAME)
            {
                // Issue a warning when the game is slowing down,
                // and reset next_update timer to prevent
                // update burst after a lag spike
                if (global_clock.getElapsedTime() > next_update)
                {
                    next_update = global_clock.getElapsedTime()
                                + sf::seconds(IWBAN_UPDATE_TIME);
                    IWBAN_LOG_WARNING("Game is slowing down!\n");
                }

                break;
            }
        }

#ifndef NDEBUG
        perf_update += global_clock.getElapsedTime();
        perf_draw -= global_clock.getElapsedTime();
#endif

        // Background rendering
        if (marginX > 0 || marginY > 0)
        {
            win.setView(bg_view);

            float t = global_clock.getElapsedTime().asSeconds();
            float t2 = - t/2;

            bg_mesh[0].texCoords.x = t;
            bg_mesh[1].texCoords.x = winW2 + t;
            bg_mesh[2].texCoords.x = winW2 + t;
            bg_mesh[3].texCoords.x = t;

            bg_mesh[0].texCoords.y = t2;
            bg_mesh[1].texCoords.y = t2;
            bg_mesh[2].texCoords.y = winH2 + t2;
            bg_mesh[3].texCoords.y = winH2 + t2;

            win.draw(bg_mesh, &bg_tex);
        }

        // Scene rendering
        win.setView(render_view);

        renderer.begin();
        projector.render(renderer);
        renderer.end();

#ifndef NDEBUG
        perf_draw += global_clock.getElapsedTime();
        perf_display -= global_clock.getElapsedTime();
#endif

        // Display on screen
        win.display();

#ifndef NDEBUG
        perf_display += global_clock.getElapsedTime();
#endif

#ifndef NDEBUG
        // Display FPS results every 1000 frames
        if ((++fps_counter) == 1000)
        {
            std::cout << "FPS : "<< (fps_counter / fps_clock.getElapsedTime().asSeconds()) << std::endl;
            fps_clock.restart();
            fps_counter = 0;
        }
        ++total_fps_counter;
#endif

    } // while(win.isOpen())

#ifndef NDEBUG
    sf::Time perf_total = global_clock.getElapsedTime();

    std::cout << "Average FPS : " << (total_fps_counter / perf_total.asSeconds()) << std::endl
              << "Time elapsed in the different sections (in %) :" << std::endl
              << std::setprecision(1) << std::fixed
              << "Events  : " << (perf_event / perf_total) * 100 << std::endl
              << "Update  : " << (perf_update / perf_total) * 100 << std::endl
              << "Draw    : " << (perf_draw / perf_total) * 100 << std::endl
              << "Display : " << (perf_display / perf_total) * 100 << std::endl
              << "Other   : " << (1 - (perf_event + perf_update + perf_draw + perf_display) / perf_total) * 100 << std::endl;
#endif
}

}
// namespace sys
