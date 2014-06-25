/**
 * @file   IntroScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INTROSCREEN_HPP_
#define _IWBAN_INTROSCREEN_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <physics/Space.hpp>
#include <physics/Body.hpp>
#include <physics/shapes/Box.hpp>

namespace sys
{

class IntroScreen : public Screen
{
protected:
    /*phy::Space sim;

    phy::Body * pl1;
    phy::Body * pl2;
    bool          pl2_j;

    phy::Body * obj1;
    phy::Body * obj2;
    phy::Body * obj3;
    phy::Body * obj4;

    phy::Box * box;
    phy::Box * box2;
    phy::Box * box3;

    std::vector<gfx::ShadowVolume *> shadows;

    gfx::ShadowVolume * shad0;
    gfx::ShadowVolume * shad1;
    gfx::ShadowVolume * shad2;

    unsigned ticks;*/

    // Callbacks
    virtual void    onUpdate(const sf::Time & update_time)
    {
        /*const float spd = 3.f;
        const float jspd = 8.f;

        ut::Vector d = pl1->getVelocity();
        d.x = 0;

        if (getControls().getAction(ACT_RIGHT).isActive())
            d.x = spd;

        if (getControls().getAction(ACT_LEFT).isActive())
            d.x = - spd;

        if (pl1->hasParent() && getControls().getAction(ACT_UP).isJustActivated())
        {
            d.y = - jspd;
        }

        pl1->setVelocity(d);

        d = pl2->getVelocity();
        d.x = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            d.x = spd;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
            d.x = - spd;

        if (pl2->hasParent())
            pl2_j = true;

        if (pl2_j && sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            d.y = - jspd;
            pl2_j = false;
        }

        pl2->setVelocity(d);

        const float pspd = 3.0f;

        if (ticks < 100)
        {
            obj3->move(ut::Vector(pspd, 0));
            obj4->move(ut::Vector(0, -pspd));
        }
        else
        {
            obj3->move(ut::Vector(-pspd, 0));
            obj4->move(ut::Vector(0, pspd));
            if (ticks == 198) ticks = 0;
        }

        sim.step();

        shad1->setPosition(obj3->getPosition());
        shad2->setPosition(obj4->getPosition());

        ++ticks;*/
    }

    virtual void    onRender(gfx::Renderer & renderer, const sf::Time & render_time) const
    {
        /*gfx::LightContext & light = renderer.openLightContext(sf::Color(64, 64, 64));
        sf::RectangleShape rect(sf::Vector2f(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT));

        light.buildShadowMask(pl1->getPosition(), shadows);
            rect.setFillColor(sf::Color(128, 128, 0));
            light.draw(rect);

        light.buildShadowMask(pl2->getPosition(), shadows);
            rect.setFillColor(sf::Color(0, 128, 128));
            light.draw(rect);

        light.disableShadowMask();

        light.close();*/

        gfx::DrawContext & draw = renderer.openDrawContext();
        draw.fill(sf::Color(28,61,12));
        draw.close();

#ifndef NDEBUG
        /*gfx::DebugContext & debug = renderer.openDebugContext();
        sim.drawDebug(debug);
        debug.close();*/
#endif
    }

    virtual void    onShow()
    {
        /*pl2_j = false;

        ticks = 0;

        ut::Rectangle rec;
        rec.x = -16;
        rec.y = -16;
        rec.w = 32;
        rec.h = 32;

        ut::Rectangle rec2;
        rec2.x = -64;
        rec2.y = -16;
        rec2.w = 128;
        rec2.h = 32;

        ut::Rectangle rec3;
        rec3.x = 0;
        rec3.y = -32;
        rec3.w = 640;
        rec3.h = 32;

        box = new phy::Box(rec);
        box2 = new phy::Box(rec2);
        box3 = new phy::Box(rec3);

        pl1 = new phy::Body(box, &phys);
        pl1->setSolidityGroup(phy::COL_PLAYER);
        pl1->setCollisionMask(phy::COL_ALL);
        pl1->setPosition(ut::Vector(150, 100));
        pl1->setMass(10.f);
        pl1->wake();

        pl2 = new phy::Body(box, &phys);
        pl2->setSolidityGroup(phy::COL_ENEMY);
        pl2->setCollisionMask(phy::COL_ALL);
        pl2->setPosition(ut::Vector(450, 100));
        pl2->setMass(3.f);
        pl2->wake();

        obj1 = new phy::Body(box3);
        obj1->setSolidityGroup(phy::COL_WORLD);
        obj1->setPosition(ut::Vector(0, 480));

        obj2 = new phy::Body(box2);
        obj2->setSolidityGroup(phy::COL_WORLD);
        obj2->setPosition(ut::Vector(150, 250));

        obj3 = new phy::Body(box2, &mov);
        obj3->setSolidityGroup(phy::COL_WORLD);
        obj3->setPosition(ut::Vector(400, 250));

        obj4 = new phy::Body(box2, &mov);
        obj4->setSolidityGroup(phy::COL_WORLD);
        obj4->setPosition(ut::Vector(300, 400));

        sim.add(pl1);
        sim.add(pl2);

        sim.add(obj1);
        sim.add(obj2);
        sim.add(obj3);
        sim.add(obj4);

        shad0 = new gfx::ShadowVolume();

        shad0->dbgAddVertex(ut::Vector(-60, -12));
        shad0->dbgAddVertex(ut::Vector(60, -12));
        shad0->dbgAddVertex(ut::Vector(60, 12));
        shad0->dbgAddVertex(ut::Vector(-60, 12));
        shad0->dbgAddVertex(ut::Vector(-60, -12));

        shad0->setPosition(ut::Vector(150, 250));

        shad1 = new gfx::ShadowVolume();

        shad1->dbgAddVertex(ut::Vector(-60, -12));
        shad1->dbgAddVertex(ut::Vector(60, -12));
        shad1->dbgAddVertex(ut::Vector(60, 12));
        shad1->dbgAddVertex(ut::Vector(-60, 12));
        shad1->dbgAddVertex(ut::Vector(-60, -12));

        shad2 = new gfx::ShadowVolume();

        shad2->dbgAddVertex(ut::Vector(-60, -12));
        shad2->dbgAddVertex(ut::Vector(60, -12));
        shad2->dbgAddVertex(ut::Vector(60, 12));
        shad2->dbgAddVertex(ut::Vector(-60, 12));
        shad2->dbgAddVertex(ut::Vector(-60, -12));

        shadows.push_back(shad0);
        shadows.push_back(shad1);
        shadows.push_back(shad2);*/
    }

    virtual void    onHide()
    {
        /*sim.remove(pl1);
        sim.remove(pl2);

        sim.remove(obj1);
        sim.remove(obj2);
        sim.remove(obj3);
        sim.remove(obj4);

        delete box;
        delete box2;
        delete box3;

        delete pl1;
        delete pl2;

        delete obj1;
        delete obj2;
        delete obj3;
        delete obj4;

        delete shad0;
        delete shad1;
        delete shad2;*/
    }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
