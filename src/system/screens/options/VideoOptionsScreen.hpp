/**
 * @file   VideoOptionsScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_VIDEOOPTIONSSCREEN_HPP_
#define _IWBAN_VIDEOOPTIONSSCREEN_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <gui/Button.hpp>
#include <gui/Label.hpp>
#include <gui/Navigation.hpp>

namespace sys
{

class VideoOptionsScreen : public Screen
{
private:
    // Data members
    gui::Navigation _navi;

    gui::Label  _title;

    gui::Button _quit;

    Screen * _previous_menu;


public:
    // Constructor
    VideoOptionsScreen(Screen * previous_menu)
        : _previous_menu(previous_menu)
    {
        // Title
        _title.loadText("options.video");
        _title.setCharacterSize(50);
        _title.setPosition({320, 50});

        // Quit
        _quit.loadText("options.return");
        _quit.setCallback([this](){ onQuit(); });
        _quit.setPosition({320, 440});

        // Navigation
        _navi.setHead(&_quit);

        /* TODO _mode = new gui::Selector();
        _mode->addEntry(0, "video.windowed");
        _mode->addEntry(1, "video.fullscreen");
        _mode->addEntry(2, "video.borderless");
        _mode->setAction([this](int mode){ cfg::fullscreen = mode; });
        _video_menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("video.mode")),
            new gui::Frame(ut::Vector(400, 40), _mode)
        }));*/

        /*{
            gui::Choice * reset = new gui::Choice("options.defaults");
            reset->setAction([this](){ / * TODO Video defaults * / });
            _video_menu->add(new gui::Separator(ut::Vector(400, 10)));
            _video_menu->add(new gui::Frame(ut::Vector(400, 40), reset));
        }*/
    }


protected:
    // Callbacks
    void onUpdate(const sf::Time & update_time) override
    {
        // If menu is pressed, return to previous menu
        if (getControls().getAction(ACT_MENU).isJustActivated())
        {
            onQuit();
            return;
        }

        for (unsigned i = 0; i < ACT_COUNT; ++i)
            if (getControls().getAction((ActionId) i).isJustActivated())
                _navi.dispatchAction((ActionId) i);
    }

    void onRender(gfx::Renderer & renderer, const sf::Time & render_time) const override
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(16, 16, 16));

        draw.close();


        gfx::GuiContext & gui = renderer.openGuiContext();

        _title.draw(gui);

        _quit.draw(gui);

        gui.close();
    }

    void onMouseMove(const ut::Vector & position) override
    {
        _navi.dispatchMouseMove(position);
    }

    void onMouseClick(const ut::Vector & position) override
    {
        _navi.dispatchMouseClick(position);
    }

    void onQuit()
    {
        setNextScreen(_previous_menu);
        _navi.reset();
    }

};
// class VideoOptionsScreen

}
// namespace sys

#endif // _IWBAN_VIDEOOPTIONSSCREEN_HPP_
