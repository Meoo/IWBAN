/**
 * @file   SplashScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SPLASHSCREEN_HPP_
#define _IWBAN_SPLASHSCREEN_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Projector.hpp>
#include <system/Screen.hpp>
#include <system/Time.hpp>

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
                          sf::Time duration = IWBAN_SPLASH_DURATION)
        : _next_screen(next_screen), _duration(duration) {}


protected:
    // Callbacks
    void onUpdate(const sf::Time & update_time) override
    {
        // Either timeout or skip
        if (update_time - _begin_time >= _duration
         || (_skippable
           && (getControls().getAction(ACT_ACCEPT).isJustActivated()
            || getControls().getAction(ACT_MENU).isJustActivated())))
        {
            getProjector()->fadeIn(sf::Color::White, sf::seconds(1));
            setNextScreen(_next_screen);
            dispose();
        }
    }

    void onRender(gfx::Renderer & renderer, const sf::Time & render_time) const override
    {
        float time_left = (_duration - render_time + _begin_time).asSeconds();

        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(196, 196, 196));

        // TODO Draw splash image

        // Fade out manually
        if (time_left < 1)
            draw.fill(sf::Color(255, 255, 255, (1 - time_left) * 255));

        draw.close();
    }

    void onShow() override
    {
        _begin_time = getGlobalTime();
    }

    void onMouseClick(const ut::Vector & position) override
    {
        if (_skippable
         && position.x > 0 && position.y > 0
         && position.x < IWBAN_FRAME_WIDTH
         && position.y < IWBAN_FRAME_HEIGHT)
        {
            getProjector()->fadeIn(sf::Color::White, sf::seconds(1));
            setNextScreen(_next_screen);
            dispose();
        }
    }

    // Accessors
    void setSkippable(bool skippable) { _skippable = skippable; }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_SPLASHSCREEN_HPP_
