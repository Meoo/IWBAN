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

    class MapLayer
    {
    public:
        std::string             name;
        bool                    enable      = true;

        std::vector<BodyPtr>    bodies;
        DrawablePtr             drawable;

    };
    // class MapLayer


    const data::Map *       _map;

    std::vector<MapLayer>   _layers;


public:
    explicit MapEntity(const data::Map * map);

    void enableLayer(const std::string & layer_name);
    void disableLayer(const std::string & layer_name);

protected:
    void doSpawn() override;
    void doDespawn() override;

    void doEvent(const std::string & event, const logic::Variant & param) override;

};
// class MapEntity

}
// namespace game

#endif // _IWBAN_MAPENTITY_HPP_
