/**
 * @file   MainMenuScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MAINMENUSCREEN_HPP_
#define _IWBAN_MAINMENUSCREEN_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <gui/Menu.hpp>
#include <gui/Choice.hpp>
#include <gui/Frame.hpp>

namespace sys
{

class MainMenuScreen : public Screen
{
private:
    // Data members
    gui::Menu * menu;


protected:
    // Callbacks
    virtual void onUpdate()
    {
        for (unsigned i = 0; i < ACT_COUNT; ++i)
            if (getControls().getAction((ActionId) i).isJustActivated())
                menu.dispatchAction((ActionId) i);
    }

    virtual void onRender(gfx::Renderer & renderer) const
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color::White);

        menu.draw(draw);

        draw.close();
    }

    virtual void onShow()
    {
        const ut::Vector choice_box(300, 40);

        menu = new gui::Menu();
        menu->setPosition(ut::Vector(0, 240));
        menu->setSize(ut::Vector(640, 200));
        menu->setCentered(true);
        menu->add(new gui::Frame(choice_box, new gui::Choice("menu.newgame")));
        menu->add(new gui::Frame(choice_box, new gui::Choice("menu.continue")));
        menu->add(new gui::Frame(choice_box, new gui::Choice("menu.options")));
        menu->add(new gui::Frame(choice_box, new gui::Choice("menu.quit")));
    }

    virtual void onHide()
    {
        delete menu;
    }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_MAINMENUSCREEN_HPP_
