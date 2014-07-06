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

#include <cstdlib>

#define N 1000

namespace sys
{

class IntroScreen : public Screen
{
protected:
    phy::Space      space;

    ut::Rectangle   rect;
    phy::Shape *    shape;

    ut::Rectangle   rect2;
    phy::Shape *    shape2;

    phy::Body *     bodies[N];

    ut::Rectangle   prect;
    phy::Shape *    pshape;

    phy::Body *     pbody;
    phy::Body *     pbody2;

    ut::Rectangle   hrect;
    phy::Shape *    hshape;

    phy::Body *     hbody;
    phy::Body *     hbody2;


    /*std::vector<gfx::ShadowVolume *> shadows;

    gfx::ShadowVolume * bod_shadows[N];*/


    // Callbacks
    virtual void    onUpdate(const sf::Time & update_time)
    {
        space.update(IWBAN_UPDATE_TIME, 4);

        /*for (unsigned i = 0; i < N; ++i)
        {
            bod_shadows[i]->setPosition(bodies[i]->getPosition());
        }*/
    }

    virtual void    onRender(gfx::Renderer & renderer, const sf::Time & render_time) const
    {
        /*sf::RectangleShape rect(sf::Vector2f(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT));

        gfx::LightContext & light = renderer.openLightContext(sf::Color(128, 128, 128));
        light.buildShadowMask(
                ut::Vector(sf::Mouse::getPosition().x,
                           sf::Mouse::getPosition().y), shadows);
        light.draw(rect);
        light.disableShadowMask();
        light.close();*/

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
        rect.h = rect.w = 10;
        rect2.h = rect2.w = 16;
        shape = new phy::Box(rect);
        shape2 = new phy::Box(rect2);

        for (unsigned i = 0; i < N; ++i)
        {
            bodies[i] = new phy::Body(i % 7 ? shape : shape2);
            bodies[i]->setPosition(ut::Vector(40 + i / 2, (i % 20) * 14 + 70));
            bodies[i]->setVelocity(ut::Vector(std::rand() % 200 - 100, std::rand() % 200 - 100));
            bodies[i]->setAcceleration({0, 150});
            bodies[i]->setMass(1);
            bodies[i]->setCollisionMask(phy::COL_WORLD | (i % 2 ? phy::COL_ENEMY : phy::COL_PLAYER));
            bodies[i]->setSolidityGroup(i % 2 ? phy::COL_ENEMY : phy::COL_PLAYER);
            space.add(bodies[i]);

            /*bod_shadows[i] = new gfx::ShadowVolume();
            bod_shadows[i]->dbgAddVertex({0, 0});
            bod_shadows[i]->dbgAddVertex({32, 0});
            bod_shadows[i]->dbgAddVertex({32, 32});
            bod_shadows[i]->dbgAddVertex({0, 32});
            bod_shadows[i]->dbgAddVertex({0, 0});
            shadows.push_back(bod_shadows[i]);*/
        }

        prect.h = 32;
        prect.w = 640;
        pshape = new phy::Box(prect);
        pbody = new phy::Body(pshape);
        pbody->setPosition({0, 480-32});
        pbody->setSolidityGroup(phy::COL_WORLD);
        pbody2 = new phy::Body(pshape);
        pbody2->setPosition({0, 0});
        pbody2->setSolidityGroup(phy::COL_WORLD);
        space.add(pbody);
        space.add(pbody2);

        hrect.h = 480 - 64;
        hrect.w = 32;
        hshape = new phy::Box(hrect);
        hbody = new phy::Body(hshape);
        hbody->setPosition({0, 32});
        hbody->setSolidityGroup(phy::COL_WORLD);
        hbody2 = new phy::Body(hshape);
        hbody2->setPosition({640 - 32, 32});
        hbody2->setSolidityGroup(phy::COL_WORLD);
        space.add(hbody);
        space.add(hbody2);
    }

    virtual void    onHide()
    {
        for (unsigned i = 0; i < N; ++i)
        {
            space.remove(bodies[i]);
            delete bodies[i];
            //delete bod_shadows[i];
        }
        delete shape;
        delete shape2;

        space.remove(pbody);
        delete pbody;
        delete pbody2;
        delete pshape;

        space.remove(hbody);
        delete hbody;
        delete hbody2;
        delete hshape;
    }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
