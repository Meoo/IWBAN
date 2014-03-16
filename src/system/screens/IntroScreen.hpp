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
protected:
    sf::CircleShape s;

    // Callbacks
    virtual void    onUpdate()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            s.setPosition(s.getPosition().x + 3.f, s.getPosition().y);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            s.setPosition(s.getPosition().x - 3.f, s.getPosition().y);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            s.setPosition(s.getPosition().x, s.getPosition().y - 3.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            s.setPosition(s.getPosition().x, s.getPosition().y + 3.f);
    }

    virtual void    onRender(gfx::Renderer & renderer) const
    {
        renderer.beginLight();
        renderer.drawLight(s);
        renderer.endLight();
    }

    virtual void    onShow()
    {
        s.setRadius(32.f);
        s.setPosition(100.f, 100.f);
        s.setFillColor(sf::Color::White);
    }
    virtual void    onHide() {}

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
