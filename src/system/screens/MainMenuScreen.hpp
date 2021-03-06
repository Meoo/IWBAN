/**
 * @file   MainMenuScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MAINMENUSCREEN_HPP_
#define _IWBAN_MAINMENUSCREEN_HPP_

#include <Global.hpp>

#include <data/Locale.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>
#include <system/screens/options/OptionsMenuScreen.hpp>
#include <system/screens/DebugScreen.hpp>

#include <gui/Button.hpp>
#include <gui/Navigation.hpp>

namespace sys
{

class MainMenuScreen : public Screen
{
private:
    // Data members
    gui::Navigation _navi;

    gui::Button _newgame, _continue, _options, _quit;


public:
    // Constructor
    MainMenuScreen()
    {
        data::getLocale().loadFile("system/menu.txt");

        _newgame.setCallback([this](){ onNewGame(); });
        _newgame.setPosition({320, 240});

        _continue.setCallback([this](){ onContinue(); });
        _continue.setPosition({320, 280});

        _options.setCallback([this](){ onOptions(); });
        _options.setPosition({320, 320});

        _quit.setCallback([this](){ onQuit(); });
        _quit.setPosition({320, 360});

        _navi.setHead(&_newgame);
        _navi.addVertical(&_newgame, &_continue);
        _navi.addVertical(&_continue, &_options);
        _navi.addVertical(&_options, &_quit);
    }


protected:
    // Callbacks
    void onShow() override
    {
        _newgame.loadText("menu.newgame");
        _continue.loadText("menu.continue");
        _options.loadText("menu.options");
        _quit.loadText("menu.quit");
    }

    void onUpdate(const sf::Time & update_time) override
    {
        for (unsigned i = 0; i < ACT_COUNT; ++i)
            if (getControls().getAction((ActionId) i).isJustActivated())
                _navi.dispatchAction((ActionId) i);
    }

    void onRender(gfx::Renderer & renderer, const sf::Time & render_time) const override
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(196, 196, 196));

        draw.close();


        gfx::GuiContext & gui = renderer.openGuiContext();

        _newgame.draw(gui);
        _continue.draw(gui);
        _options.draw(gui);
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

    void onNewGame()
    {
        // TODO Go to new game screen
        setNextScreen(new DebugScreen());
        dispose();
    }

    void onContinue()
    {
        // TODO Continue?
        _continue.setTextColor(sf::Color::Red);
    }

    void onOptions()
    {
        setNextScreen(new OptionsMenuScreen(this));
    }

    void onQuit()
    {
        // TODO Quit
        _quit.setTextColor(sf::Color::Red);
    }

};
// class MainMenuScreen

}
// namespace sys

#endif // _IWBAN_MAINMENUSCREEN_HPP_
