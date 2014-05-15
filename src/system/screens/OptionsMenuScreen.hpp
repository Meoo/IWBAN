/**
 * @file   OptionsMenuScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OPTIONSMENUSCREEN_HPP_
#define _IWBAN_OPTIONSMENUSCREEN_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <gui/Choice.hpp>
#include <gui/Frame.hpp>
#include <gui/Line.hpp>
#include <gui/Menu.hpp>
#include <gui/Separator.hpp>

namespace sys
{

class OptionsMenuScreen : public Screen
{
private:
    // Data members
    gui::Menu * _menu;


public:
    // Constructor
    OptionsMenuScreen()
    {
        const ut::Vector choice_box(300, 40);

        res::getLocale().loadFile("system/menu.txt");

        _menu = new gui::Menu();
        _menu->setPosition(ut::Vector(0, 20));
        _menu->setSize(ut::Vector(640, 400));
        _menu->setCentered(true);

        {
            gui::Label * title = new gui::Label("menu.options");
            title->setCharacterSize(40);
            _menu->add(new gui::Frame(ut::Vector(400, 80), title));
        }

        _menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("menu.options.volume")),
            new gui::Frame(ut::Vector(400, 40), new gui::Label("VOLUME"))
        }));

        _menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("menu.options.gamepad")),
            new gui::Separator(ut::Vector(400, 40))
        }));

        _menu->add(new gui::Line({
            new gui::Frame(ut::Vector(200, 40), new gui::Choice("menu.options.keyboard")),
            new gui::Separator(ut::Vector(400, 40))
        }));

        _menu->add(new gui::Separator(ut::Vector(400, 20)));
        _menu->add(new gui::Frame(ut::Vector(400, 40), new gui::Choice("menu.options.save")));
        _menu->add(new gui::Frame(ut::Vector(400, 40), new gui::Choice("menu.options.cancel")));

    }

    // Destructor
    virtual ~OptionsMenuScreen()
    {
        // Choices are released by _menu
        delete _menu;
    }


protected:
    // Callbacks
    virtual void onUpdate()
    {
        if (getControls().getAction(ACT_MENU).isJustActivated())
        {
            // TODO Go back to main menu
            return;
        }

        for (unsigned i = 0; i < ACT_COUNT; ++i)
            if (getControls().getAction((ActionId) i).isJustActivated())
                _menu->dispatchAction((ActionId) i);
    }

    virtual void onRender(gfx::Renderer & renderer) const
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(32, 32, 32));

        _menu->draw(draw);

        draw.close();
    }

    virtual void onShow()
    {
    }

    virtual void onHide()
    {
    }

};
// class OptionsMenuScreen

}
// namespace sys

#endif // _IWBAN_OPTIONSMENUSCREEN_HPP_
