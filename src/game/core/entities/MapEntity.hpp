/**
 * @file   MapEntity.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MAPENTITY_HPP_
#define _IWBAN_MAPENTITY_HPP_

#include <Global.hpp>

#include <data/Map.hpp>
#include <logic/Entity.hpp>

namespace game
{

class MapEntity final : public logic::Entity
{
private:
    typedef data::map::BodyDesc::BodyPtr    BodyPtr;
    typedef data::map::Layer::DrawablePtr   DrawablePtr;

    const data::Map *       _map;

    std::vector<BodyPtr>    _map_bodies;
    std::vector<DrawablePtr> _map_drawables;


public:
    explicit MapEntity(const data::Map * map)
        : _map(map) {}


protected:
    void doSpawn() override;

};
// class MapEntity

}
// namespace game

#endif // _IWBAN_MAPENTITY_HPP_
