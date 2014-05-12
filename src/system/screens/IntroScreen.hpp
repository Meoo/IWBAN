/**
 * @file   IntroScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INTROSCREEN_HPP_
#define _IWBAN_INTROSCREEN_HPP_

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Screen.hpp>

#include <physics/behaviors/PhysicsBehavior.hpp>
#include <physics/behaviors/MovingBehavior.hpp>
#include <physics/Simulator.hpp>
#include <physics/Object.hpp>
#include <physics/shapes/Box.hpp>

#include <resources/File.hpp>
#include <resources/Locale.hpp>

#include <gui/Menu.hpp>
#include <gui/Choice.hpp>
#include <gui/Frame.hpp>

namespace sys
{

class IntroScreen : public Screen
{
protected:
    phy::Simulator sim;

    phy::Object * pl1;
    phy::Object * pl2;
    bool          pl2_j;

    phy::Object * obj1;
    phy::Object * obj2;
    phy::Object * obj3;
    phy::Object * obj4;

    phy::Box * box;
    phy::Box * box2;
    phy::Box * box3;

    phy::PhysicsBehavior phys;
    phy::MovingBehavior mov;

    gui::Menu menu;

    std::vector<gfx::ShadowVolume *> shadows;

    gfx::ShadowVolume * shad0;
    gfx::ShadowVolume * shad1;
    gfx::ShadowVolume * shad2;

    unsigned ticks;

    // Callbacks
    virtual void    onUpdate()
    {
        const float spd = 3.f;
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

        for (unsigned i = 0; i < ACT_COUNT; ++i)
            if (getControls().getAction((ActionId) i).isJustActivated())
                menu.dispatchAction((ActionId) i);

        shad1->setPosition(obj3->getPosition());
        shad2->setPosition(obj4->getPosition());

        ++ticks;
    }

    virtual void    onRender(gfx::Renderer & renderer) const
    {
        gfx::LightContext & light = renderer.openLightContext(sf::Color(64, 64, 64));
        sf::RectangleShape rect(sf::Vector2f(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT));

        light.buildShadowMask(pl1->getPosition(), shadows);

        rect.setFillColor(sf::Color(128, 128, 0));
        light.draw(rect);

        light.buildShadowMask(pl2->getPosition(), shadows);

        rect.setFillColor(sf::Color(0, 128, 128));
        light.draw(rect);

        light.disableShadowMask();

        light.close();

        gfx::DrawContext & draw = renderer.openDrawContext();

        draw.fill(sf::Color(28,61,12));

        sf::String testext(res::getLocale().getString("test"));
        sf::Text text(testext, res::getLocale().getFont(), 24);
        text.setPosition(25, 25);
        text.setColor(sf::Color::Black);
        draw.draw(text);

        menu.draw(draw);

        draw.close();

#ifndef NDEBUG
        gfx::DebugContext & debug = renderer.openDebugContext();
        sim.drawDebug(debug);
        text.setPosition(20, 20);
        text.setColor(sf::Color::Red);
        debug.draw(text);
        debug.close();
#endif
    }

    virtual void    onShow()
    {
        res::getLocale().loadFile("system/language.txt");
        res::getLocale().loadFile("system/menu.txt");
        res::getLocale().loadFont("system/poetsen_one.ttf");

        menu.setPosition(ut::Vector(0, 240));
        menu.setSize(ut::Vector(640, 240));
        menu.setCentered(true);
        menu.add(new gui::Frame(ut::Vector(300, 40), new gui::Choice("menu.newgame")));
        menu.add(new gui::Frame(ut::Vector(300, 40), new gui::Choice("menu.continue")));
        menu.add(new gui::Frame(ut::Vector(300, 40), new gui::Choice("menu.options")));
        menu.add(new gui::Frame(ut::Vector(300, 40), new gui::Choice("menu.quit")));

        pl2_j = false;

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

        pl1 = new phy::Object(box, &phys);
        pl1->setSolidityGroup(phy::COL_PLAYER);
        pl1->setCollisionMask(phy::COL_ALL);
        pl1->setPosition(ut::Vector(150, 100));
        pl1->setMass(10.f);
        pl1->wake();

        pl2 = new phy::Object(box, &phys);
        pl2->setSolidityGroup(phy::COL_ENEMY);
        pl2->setCollisionMask(phy::COL_ALL);
        pl2->setPosition(ut::Vector(450, 100));
        pl2->setMass(3.f);
        pl2->wake();

        obj1 = new phy::Object(box3);
        obj1->setSolidityGroup(phy::COL_WORLD);
        obj1->setPosition(ut::Vector(0, 480));

        obj2 = new phy::Object(box2);
        obj2->setSolidityGroup(phy::COL_WORLD);
        obj2->setPosition(ut::Vector(150, 250));

        obj3 = new phy::Object(box2, &mov);
        obj3->setSolidityGroup(phy::COL_WORLD);
        obj3->setPosition(ut::Vector(400, 250));

        obj4 = new phy::Object(box2, &mov);
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
        shadows.push_back(shad2);
    }

    virtual void    onHide()
    {
        sim.remove(pl1);
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
        delete shad2;
    }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
