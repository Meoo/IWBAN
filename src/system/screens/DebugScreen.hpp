/**
 * @file   DebugScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INTROSCREEN_HPP_
#define _IWBAN_INTROSCREEN_HPP_

#include <Global.hpp>

#include <data/Map.hpp>

#include <game/core/Save.hpp>
#include <game/core/entities/MapEntity.hpp>
#include <game/core/entities/PlayerEntity.hpp>
#include <game/world1/entities/BouncyBallEntity.hpp>

#include <logic/World.hpp>

#include <resources/Async.hpp>

#include <system/Screen.hpp>

#define N 50

namespace sys
{

class DebugScreen : public Screen
{
public:
    DebugScreen() {}


protected:
    gfx::CameraOperator     camera_op;

    data::Map *             test_map    = nullptr;

    logic::Entity *         map_entity  = nullptr;
    game::PlayerEntity *    player      = nullptr;
    game::BouncyBallEntity * ball       = nullptr;

    sf::Time                last_update;

    game::Save *            save        = nullptr;
    game::LuaSave *         lua_save    = nullptr;


    // Callbacks
    void onUpdate(const sf::Time & update_time) override
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            delete map_entity;

            test_map = data::getMap("test/maps/test2.map");
            map_entity = new game::MapEntity(test_map);
            map_entity->setName("map");
        }

        /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            map_entity->sendEvent("test");
        }*/

        logic::getWorld().update();

        camera_op.setTarget(player->getPosition());

        last_update = update_time;

#ifndef NDEBUG
        // Poll standard input and run commands on Lua
        std::string sin;
        while (res::async::pollStdInput(sin))
        {
            if (sin.at(0) == '=')
                sin = "print(" + sin.substr(1) + ")";

            logic::getWorld().getLua().run(logic::LuaScript("stdin", sin));
        }
#endif
    }

    void onRender(gfx::Renderer & renderer, const sf::Time & render_time) const override
    {
        // TODO float interpolation_ratio = (render_time - last_update) / IWBAN_UPDATE_TIME;

        renderer.setCamera(camera_op.getCameraAt(render_time));

        logic::getWorld().getScene().draw(renderer);


#ifndef NDEBUG
        gfx::DebugContext & debug = renderer.openDebugContext();

        logic::getWorld().getSpace().drawDebug(debug);

        debug.close();
#endif
    }

    void onShow() override
    {
        logic::Lua & l = logic::getWorld().getLua();

        save = new game::Save("iwban", 1);
        save->load();
        lua_save = new game::LuaSave(save->createLuaSave(l));

        l.pushObject(lua_save->getLuaObject());
        lua_setglobal(l, "save");

        test_map = data::getMap("test/maps/test.map");

        map_entity = new game::MapEntity(test_map);
        player     = new game::PlayerEntity();
        ball       = new game::BouncyBallEntity();

        player->setName("player");
        map_entity->setName("map");
        ball->setName("ball");

        player->setPosition(ut::Vector(75, 75));
    }

    void onHide() override
    {
        delete lua_save;
        save->save();
        delete save;

        delete map_entity;
        delete player;
        delete ball;
    }

    void onMouseClick(const ut::Vector & position)
    {
        player->setPosition(camera_op.getCamera().screenToWorld(position));
    }

};
// class DebugScreen

}
// namespace sys

#endif // _IWBAN_INTROSCREEN_HPP_
