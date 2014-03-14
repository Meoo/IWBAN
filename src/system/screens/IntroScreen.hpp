/**
 * @file   IntroScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INTROSCREEN_HPP_
#define _IWBAN_INTROSCREEN_HPP_

#include <Global.hpp>

#include <system/Screen.hpp>

namespace sys
{

class IntroScreen : public Screen
{
    sf::Texture tex;

protected:
    // Callbacks
    virtual void    onUpdate() {}
    virtual void    onRender(gfx::Renderer & renderer) const
    {
        renderer.beginLight();
        static sf::CircleShape s(256);
        s.setFillColor(sf::Color::White);
        renderer.drawLight(s);
        renderer.endLight();
    }

    virtual void    onShow() {}
    virtual void    onHide() {}

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
