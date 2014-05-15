/**
 * @file   SplashScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SPLASHSCREEN_HPP_
#define _IWBAN_SPLASHSCREEN_HPP_

#include <Global.hpp>

#include <system/Clock.hpp>
#include <system/Controls.hpp>
#include <system/Screen.hpp>

namespace sys
{

class SplashScreen : public Screen
{
private:
    // Data members
    Screen * _next_screen;
    bool     _skippable = true;

    sf::Time _begin_time;
    sf::Time _duration;


public:
    // Constructor
    explicit SplashScreen(Screen * next_screen,
                          sf::Time duration = sf::seconds(IWBAN_SPLASH_DURATION))
        : _next_screen(next_screen), _duration(duration) {}


protected:
    // Callbacks
    virtual void onUpdate()
    {
        // Either timeout or skip
        if (getUpdateTime() - _begin_time >= _duration
         || (_skippable
           && (getControls().getAction(ACT_ACCEPT).isJustActivated()
            || getControls().getAction(ACT_CANCEL).isJustActivated()
            || getControls().getAction(ACT_MENU).isJustActivated())))
        {
            setNextScreen(_next_screen);
            dispose();
        }
    }

    virtual void onRender(gfx::Renderer & renderer) const
    {
        float time_left = (_duration - getDrawTime() + _begin_time).asSeconds();

        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color::White);

        // TODO Draw splash image

        if (time_left < 1)
            draw.fill(sf::Color(0, 0, 0, (1 - time_left) * 255));

        draw.close();
    }

    virtual void onShow()
    {
        _begin_time = getUpdateTime();
    }

    virtual void onHide()
    {
    }

    // Accessors
    void setSkippable(bool skippable) { _skippable = skippable; }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_SPLASHSCREEN_HPP_
