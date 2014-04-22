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
    phy::Object * obj;
    phy::Object * obj2;
    phy::Object * obj3;
    phy::Box * box;
    phy::Box * box2;
    phy::PhysicsBehavior behavior;

    // Callbacks
    virtual void    onUpdate()
    {
        const float spd = 3.f;

        ut::Vector d = obj->getVelocity();
        d.x = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            d.x = spd;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            d.x = - spd;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            d.y = - 6;

        obj->setVelocity(d);

        sim.step();
    }

    virtual void    onRender(gfx::Renderer & renderer) const
    {
        // Debug
        gfx::DrawContext & debug = renderer.openDebugContext();

        sim.drawDebug(debug);

        debug.close();
    }

    virtual void    onShow()
    {
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

        obj = new phy::Object(box, &behavior);
        obj->setPosition(ut::Vector(200, 100));
        obj->wake();

        obj2 = new phy::Object(box2);
        obj2->setPosition(ut::Vector(200, 250));

        obj3 = new phy::Object(box2);
        obj3->setPosition(ut::Vector(450, 250));

        sim.add(*obj);
        sim.add(*obj2);
        sim.add(*obj3);
    }
    virtual void    onHide()
    {
        sim.remove(*obj);
        sim.remove(*obj2);
        sim.remove(*obj3);

        delete box;
        delete box2;
        delete obj;
        delete obj2;
        delete obj3;
    }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
