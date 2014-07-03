/**
 * @file   IntroScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INTROSCREEN_HPP_
#define _IWBAN_INTROSCREEN_HPP_

#include <Global.hpp>

#include <config/DisplayConfig.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <physics/Space.hpp>
#include <physics/Body.hpp>
#include <physics/shapes/Box.hpp>

#define N 98

namespace sys
{

class IntroScreen : public Screen
{
protected:
    phy::Space      space;

    ut::Rectangle   rect;
    phy::Shape *    shape;

    phy::Body *     bodies[N];

    ut::Rectangle   prect;
    phy::Shape *    pshape;

    phy::Body *     pbody;


    std::vector<gfx::ShadowVolume *> shadows;

    gfx::ShadowVolume * bod_shadows[N];


    // Callbacks
    virtual void    onUpdate(const sf::Time & update_time)
    {
        space.update(IWBAN_UPDATE_TIME, 4);

        for (unsigned i = 0; i < N; ++i)
        {
            bod_shadows[i]->setPosition(bodies[i]->getPosition());
        }
    }

    virtual void    onRender(gfx::Renderer & renderer, const sf::Time & render_time) const
    {
        sf::RectangleShape rect(sf::Vector2f(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT));

        gfx::LightContext & light = renderer.openLightContext(sf::Color(128, 128, 128));
        light.buildShadowMask(
                ut::Vector(sf::Mouse::getPosition().x,
                           sf::Mouse::getPosition().y), shadows);
        light.draw(rect);
        light.disableShadowMask();
        light.close();

        gfx::DrawContext & draw = renderer.openDrawContext();
        draw.fill(sf::Color(28,61,12));
        draw.close();

#ifndef NDEBUG
        gfx::DebugContext & debug = renderer.openDebugContext();
        space.drawDebug(debug);
        debug.close();
#endif
    }

    virtual void    onShow()
    {
        rect.h = rect.w = 32;
        shape = new phy::Box(rect);

        for (unsigned i = 0; i < N; ++i)
        {
            bodies[i] = new phy::Body(shape);
            bodies[i]->setPosition(ut::Vector(5 + 6 * i, (i % 7) * 40 + 60));
            bodies[i]->setVelocity({0, -100});
            bodies[i]->setAcceleration({0, 150});
            bodies[i]->setMass(1);
            space.add(bodies[i]);

            bod_shadows[i] = new gfx::ShadowVolume();
            bod_shadows[i]->dbgAddVertex({0, 0});
            bod_shadows[i]->dbgAddVertex({32, 0});
            bod_shadows[i]->dbgAddVertex({32, 32});
            bod_shadows[i]->dbgAddVertex({0, 32});
            bod_shadows[i]->dbgAddVertex({0, 0});
            shadows.push_back(bod_shadows[i]);
        }

        prect.h = 32;
        prect.w = 640;
        pshape = new phy::Box(prect);
        pbody = new phy::Body(pshape);
        pbody->setPosition({0, 480-32});
        space.add(pbody);
    }

    virtual void    onHide()
    {
        for (unsigned i = 0; i < N; ++i)
        {
            space.remove(bodies[i]);
            delete bodies[i];
            delete bod_shadows[i];
        }
        delete shape;

        space.remove(pbody);
        delete pbody;
        delete pshape;
    }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
