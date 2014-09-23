/**
 * @file   MapEntity.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <game/core/entities/MapEntity.hpp>

namespace game
{

void MapEntity::doSpawn()
{
    for (auto & layer : _map->getLayers())
    {
        DrawablePtr drawable = layer->makeDrawable();
        addDrawable(drawable.get());
        _map_drawables.push_back(std::move(drawable));

        for (auto & bodydesc : layer->getBodies())
        {
            BodyPtr body = bodydesc->makeBody();
            addBody(body.get());
            _map_bodies.push_back(std::move(body));
        }
    }
}

}
// namespace game
