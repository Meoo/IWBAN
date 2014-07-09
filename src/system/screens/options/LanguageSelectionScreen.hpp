/**
 * @file   LanguageSelectionScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LANGUAGESELECTIONSCREEN_HPP_
#define _IWBAN_LANGUAGESELECTIONSCREEN_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <gui/Button.hpp>
#include <gui/Label.hpp>
#include <gui/Navigation.hpp>

namespace sys
{

class LanguageSelectionScreen : public Screen
{
private:
    // Data members
    gui::Navigation _navi;

    gui::Label  _title;

    gui::Button _quit;

    Screen * _previous_menu;


public:
    // Constructor
    LanguageSelectionScreen(Screen * previous_menu)
        : _previous_menu(previous_menu)
    {
        // Title
        _title.loadText("options.language");
        _title.setCharacterSize(50);
        _title.setPosition({320, 50});

        // Quit
        _quit.loadText("options.return");
        _quit.setCallback([this](){ onQuit(); });
        _quit.setPosition({320, 440});

        // Navigation
        _navi.setHead(&_quit);

    }

    // Destructor
    virtual ~LanguageSelectionScreen() {}


protected:
    // Callbacks
    virtual void onUpdate(const sf::Time & update_time)
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

    virtual void onRender(gfx::Renderer & renderer, const sf::Time & render_time) const
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(16, 16, 16));

        draw.close();


        gfx::GuiContext & gui = renderer.openGuiContext();

        _title.draw(gui);

        _quit.draw(gui);

        gui.close();
    }

    virtual void onMouseMove(const ut::Vector & position)
    {
        _navi.dispatchMouseMove(position);
    }

    virtual void onMouseClick(const ut::Vector & position)
    {
        _navi.dispatchMouseClick(position);
    }

    void onQuit()
    {
        setNextScreen(_previous_menu);
        _navi.reset();
    }

};
// class LanguageSelectionScreen

}
// namespace sys

#endif // _IWBAN_LANGUAGESELECTIONSCREEN_HPP_
