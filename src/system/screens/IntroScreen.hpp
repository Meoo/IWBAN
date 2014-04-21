/**
 * @file   IntroScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INTROSCREEN_HPP_
#define _IWBAN_INTROSCREEN_HPP_

#include <Global.hpp>

#include <system/Screen.hpp>

#include <physics/Behavior.hpp>
#include <physics/Simulator.hpp>
#include <physics/Object.hpp>
#include <physics/shapes/Box.hpp>

class TestBehavior : public phy::Behavior
{
public:
    virtual void step(phy::Object & object)
    {
        ut::Vector vel = object.getVelocity();

        vel.y += 0.3f;
        object.setVelocity(vel);
        object.move(vel);
    }

    virtual void stepChild(const phy::Object & object, phy::Object & child)
    {
        child.move(object.getVelocity());
        child.wake();
    }

    virtual void onCollide(phy::Object & object, phy::Object & other, const phy::CollisionData & data)
    {
        ut::Vector delta_vel = other.getVelocity() - object.getVelocity();

        if (std::abs(data.intersect.x - delta_vel.x)
          > std::abs(data.intersect.y - delta_vel.y))
        {
            object.move(ut::Vector(0, data.intersect.y));

            ut::Vector vel = object.getVelocity();
            vel.y = 0;
            object.setVelocity(vel);
        }
        else
        {
            object.move(ut::Vector(data.intersect.x, 0));

            ut::Vector vel = object.getVelocity();
            vel.x = 0;
            object.setVelocity(vel);
        }
    }

    virtual void prepare(phy::Object &)
    {
    }
    virtual void free(phy::Object &)
    {
    }
};

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
    TestBehavior behavior;

    // Callbacks
    virtual void    onUpdate()
    {
        const float spd = 3.f;

        ut::Vector d;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            d.x = spd;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            d.x = - spd;

        obj->move(d);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            obj->setVelocity(ut::Vector(0, -6));

        sim.step();
    }

    virtual void    onRender(gfx::Renderer & renderer) const
    {
        // Color
        gfx::DrawContext & draw = renderer.openDrawContext();

        sim.drawDebug(draw);

        draw.close();
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
