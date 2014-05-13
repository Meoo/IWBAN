/**
 * @file   OptionsMenuScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OPTIONSMENUSCREEN_HPP_
#define _IWBAN_OPTIONSMENUSCREEN_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <gui/Menu.hpp>
#include <gui/Choice.hpp>
#include <gui/Frame.hpp>

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
        _menu->setPosition(ut::Vector(0, 40));
        _menu->setSize(ut::Vector(640, 400));
        _menu->setCentered(true);

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
