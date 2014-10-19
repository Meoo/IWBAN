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

void MapEntity::enableLayer(const std::string & layer_name)
{
    for (MapLayer & layer : _layers)
    {
        if (!layer.enable && layer.name == layer_name)
        {
            layer.enable = true;

            if (isSpawned())
            {
                addDrawable(layer.drawable.get());

                for (BodyPtr & body : layer.bodies)
                    addBody(body.get());
            }
        }
    }
}

void MapEntity::disableLayer(const std::string & layer_name)
{
    for (MapLayer & layer : _layers)
    {
        if (layer.enable && layer.name == layer_name)
        {
            layer.enable = false;

            if (isSpawned())
            {
                removeDrawable(layer.drawable.get());

                for (BodyPtr & body : layer.bodies)
                    removeBody(body.get());
            }
        }
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
        if (!layer.enable)
            continue;

        addDrawable(layer.drawable.get());

        for (BodyPtr & body : layer.bodies)
            addBody(body.get());
    }
}

void MapEntity::doDespawn()
{
    for (MapLayer & layer : _layers)
    {
        if (!layer.enable)
            continue;

        removeDrawable(layer.drawable.get());

        for (BodyPtr & body : layer.bodies)
            removeBody(body.get());
    }
}

void MapEntity::doEvent(const std::string & event, const logic::Variant & param)
{
    if (event == "enableLayer")
    {
        if (param.isString())
            enableLayer(param.toString());

        return;
    }

    if (event == "disableLayer")
    {
        if (param.isString())
            disableLayer(param.toString());

        return;
    }

    Entity::doEvent(event, param);
}

}
// namespace game
