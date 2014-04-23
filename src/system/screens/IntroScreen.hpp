/**
 * @file   IntroScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INTROSCREEN_HPP_
#define _IWBAN_INTROSCREEN_HPP_

#include <Global.hpp>

#include <system/Screen.hpp>

#include <physics/behaviors/PhysicsBehavior.hpp>
#include <physics/Simulator.hpp>
#include <physics/Object.hpp>
#include <physics/shapes/Box.hpp>

namespace sys
{

class IntroScreen : public Screen
{
protected:
    phy::Simulator sim;
    phy::Object * pl1;
    phy::Object * pl2;
    phy::Object * obj2;
    phy::Object * obj3;
    phy::Object * obj4;
    phy::Box * box;
    phy::Box * box2;
    phy::PhysicsBehavior behavior;

    unsigned ticks;

    // Callbacks
    virtual void    onUpdate()
    {
        const float spd = 3.f;

        ut::Vector d = pl1->getVelocity();
        d.x = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            d.x = spd;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            d.x = - spd;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            d.y = - 6;

        pl1->setVelocity(d);

        d = pl2->getVelocity();
        d.x = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            d.x = spd;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
            d.x = - spd;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            d.y = - 6;

        pl2->setVelocity(d);

        if (ticks < 100)
        {
            obj3->move(ut::Vector(1, 0));
            obj4->move(ut::Vector(0, -1));
        }
        else
        {
            obj3->move(ut::Vector(-1, 0));
            obj4->move(ut::Vector(0, 1));
            if (ticks == 200) ticks = 0;
        }

        sim.step();

        ++ticks;
    }

    virtual void    onRender(gfx::Renderer & renderer) const
    {
        // Debug
        gfx::DrawContext & debug = renderer.openDebugContext();

        debug.fill(sf::Color(5,25,50));
        sim.drawDebug(debug);

        debug.close();
    }

    virtual void    onShow()
    {
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

        box = new phy::Box(rec);
        box2 = new phy::Box(rec2);

        pl1 = new phy::Object(box, &behavior);
        pl1->setPosition(ut::Vector(150, 100));
        pl1->wake();

        pl2 = new phy::Object(box, &behavior);
        pl2->setPosition(ut::Vector(450, 100));
        pl2->wake();

        obj2 = new phy::Object(box2);
        obj2->setPosition(ut::Vector(150, 250));

        obj3 = new phy::Object(box2);
        obj3->setPosition(ut::Vector(450, 250));

        obj4 = new phy::Object(box2);
        obj4->setPosition(ut::Vector(300, 400));

        sim.add(*pl1);
        sim.add(*pl2);

        sim.add(*obj2);
        sim.add(*obj3);
        sim.add(*obj4);
    }

    virtual void    onHide()
    {
        sim.remove(*pl1);
        sim.remove(*pl2);

        sim.remove(*obj2);
        sim.remove(*obj3);
        sim.remove(*obj4);

        delete box;
        delete box2;

        delete pl1;
        delete pl2;

        delete obj2;
        delete obj3;
        delete obj4;
    }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
