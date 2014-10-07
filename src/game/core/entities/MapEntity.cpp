/**
 * @file   MapEntity.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <game/core/entities/MapEntity.hpp>

namespace game
{

MapEntity::MapEntity(const data::Map * map)
    : _map(map)
{
    for (auto & layer : _map->getLayers())
    {
        MapLayer l;
        l.name = layer->getName();

        DrawablePtr drawable = layer->makeDrawable();
        l.drawable = std::move(drawable);

        for (auto & bodydesc : layer->getBodies())
        {
            BodyPtr body = bodydesc->makeBody();
            l.bodies.push_back(std::move(body));
        }

        _layers.push_back(std::move(l));
    }
}

void MapEntity::doSpawn()
{
    logic::Lua & l = getLua();

    if (_map->getLuaScript().isValid())
    {
        l.pushObject(getLuaObject());
        lua_setglobal(l, "MAP");

        l.run(_map->getLuaScript());

        lua_pushnil(l);
        lua_setglobal(l, "MAP");
    }

    for (MapLayer & layer : _layers)
    {
        addDrawable(layer.drawable.get());

        for (BodyPtr & body : layer.bodies)
            addBody(body.get());
    }
}

}
// namespace game
