/**
 * @file   ExceptionScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_EXCEPTIONSCREEN_HPP_
#define _IWBAN_EXCEPTIONSCREEN_HPP_

#include <Global.hpp>

#include <gui/Label.hpp>
#include <system/Screen.hpp>

namespace sys
{

class ExceptionScreen : public Screen
{
private:
    // Data member
    gui::Label  _title;
    gui::Label  _error;
    gui::Label  _info_text;


public:
    // Constructor
    ExceptionScreen(const std::string & title, const std::string & what)
    {
        _title.setText(title);
        _title.setCharacterSize(50);
        _title.setPosition({320, 120});

        _error.setText(what);
        _error.setPosition({320, 240});

        // TODO Better exception info message
        _info_text.setText("An error made the game crash !");
        _info_text.setCharacterSize(20);
        _info_text.setPosition({320, 440});
    }


protected:
    // Callbacks
    void onUpdate(const sf::Time & update_time) override
    {
    }

    void onRender(gfx::Renderer & renderer, const sf::Time & render_time) const override
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(35, 35, 220));

        draw.close();


        gfx::GuiContext & gui = renderer.openGuiContext();

        _title.draw(gui);
        _error.draw(gui);
        _info_text.draw(gui);

        gui.close();
    }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_EXCEPTIONSCREEN_HPP_
