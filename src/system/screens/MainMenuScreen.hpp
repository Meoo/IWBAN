/**
 * @file   MainMenuScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MAINMENUSCREEN_HPP_
#define _IWBAN_MAINMENUSCREEN_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>
#include <system/screens/OptionsMenuScreen.hpp>
#include <system/screens/IntroScreen.hpp>

#include <gui/Menu.hpp>
#include <gui/Choice.hpp>
#include <gui/Frame.hpp>

#include <resources/Locale.hpp>

namespace sys
{

class MainMenuScreen : public Screen
{
private:
    // Data members
    gui::Menu * _menu;

    gui::Choice * _newgame, * _continue, * _options, * _quit;


public:
    // Constructor
    MainMenuScreen()
    {
        const ut::Vector choice_box(300, 40);

        res::getLocale().loadFile("system/menu.txt");

        _menu = new gui::Menu();
        _menu->setPosition(ut::Vector(0, 240));
        _menu->setSize(ut::Vector(640, 200));
        _menu->setCentered(true);

        _menu->add(new gui::Frame(choice_box, _newgame = new gui::Choice("menu.newgame")));
        _menu->add(new gui::Frame(choice_box, _continue = new gui::Choice("menu.continue")));
        _menu->add(new gui::Frame(choice_box, _options = new gui::Choice("menu.options")));
        _menu->add(new gui::Frame(choice_box, _quit = new gui::Choice("menu.quit")));

        _newgame->setAction([this](){ onNewGame(); });
        _continue->setAction([this](){ onContinue(); });
        _options->setAction([this](){ onOptions(); });
        _quit->setAction([this](){ onQuit(); });
    }

    // Destructor
    virtual ~MainMenuScreen()
    {
        // Choices are released by _menu
        delete _menu;
    }


protected:
    // Callbacks
    virtual void onUpdate(const sf::Time & update_time)
    {
        for (unsigned i = 0; i < ACT_COUNT; ++i)
            if (getControls().getAction((ActionId) i).isJustActivated())
                _menu->dispatchAction((ActionId) i);
    }

    virtual void onRender(gfx::Renderer & renderer, const sf::Time & render_time) const
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(196, 196, 196));

        _menu->draw(draw);

        draw.close();
    }

    void onNewGame()
    {
        _newgame->setTextColor(sf::Color::Red);

        // TODO Go to new game screen
        setNextScreen(new IntroScreen());
        dispose();
    }

    void onContinue()
    {
        _continue->setTextColor(sf::Color::Red);
    }

    void onOptions()
    {
        _options->setTextColor(sf::Color::Red);

        // TODO Go to options screen
        setNextScreen(new OptionsMenuScreen());
        dispose();
    }

    void onQuit()
    {
        _quit->setTextColor(sf::Color::Red);
    }

};
// class MainMenuScreen

}
// namespace sys

#endif // _IWBAN_MAINMENUSCREEN_HPP_
