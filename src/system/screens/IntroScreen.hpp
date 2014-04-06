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
    sf::CircleShape s2;
    sf::RectangleShape r;

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
        {
            gfx::LightContext & light = renderer.openLightContext(sf::Color::Black);
            light.draw(s);
            light.draw(s2);
        }
        {
            gfx::DrawContext & draw = renderer.openDrawContext();
            draw.draw(r);
        }
    }

    virtual void    onShow()
    {
        s.setRadius(64.f);
        s.setPosition(100.f, 100.f);
        s.setFillColor(sf::Color::White);

        s2.setRadius(196.f);
        s2.setPosition(200.f, 150.f);
        s2.setFillColor(sf::Color::Magenta);

        r.setSize(sf::Vector2f(200, 200));
        r.setPosition(200.f, 160.f);
        r.setFillColor(sf::Color::Cyan);
    }
    virtual void    onHide() {}

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
