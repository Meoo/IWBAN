/**
 * @file   IntroScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INTROSCREEN_HPP_
#define _IWBAN_INTROSCREEN_HPP_

#include <Global.hpp>

#include <data/Map.hpp>

#include <game/core/entities/MapEntity.hpp>
#include <game/core/entities/PlayerEntity.hpp>

#include <logic/World.hpp>

#include <system/Screen.hpp>

#define N 50

namespace sys
{

class IntroScreen : public Screen
{
public:
    IntroScreen() {}


protected:
    data::Map * test_map;

    logic::Entity * map_entity;
    logic::Entity * player;


    // Callbacks
    virtual void    onUpdate(const sf::Time & update_time)
    {
        logic::getWorld().update();
    }

    virtual void    onRender(gfx::Renderer & renderer, const sf::Time & render_time) const
    {
        gfx::DrawContext & draw = renderer.openDrawContext();

        logic::getWorld().testRender(draw);

        draw.close();

#ifndef NDEBUG
        gfx::DebugContext & debug = renderer.openDebugContext();

        logic::getWorld().getSpace().drawDebug(debug);

        debug.close();
#endif
    }

    virtual void    onShow()
    {
        test_map = data::getMap("test/maps/test.map");

        map_entity = new game::MapEntity(test_map);
        player     = new game::PlayerEntity();
    }

    virtual void    onHide()
    {
        delete map_entity;
        delete player;
    }

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
