/**
 * @file   Map.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MAP_HPP_
#define _IWBAN_MAP_HPP_

#include <Global.hpp>

#include <SFML/Graphics.hpp>

#include <map>
#include <vector>

namespace game
{

class Map
{
public:
    class EntityDescriptor
    {
    public:
        typedef std::map<std::string, std::string> PropertyMap;

        std::string     class_name;
        std::string     name;

        PropertyMap     properties;

    };
    // class EntityDescriptor

    typedef std::vector<EntityDescriptor> DescriptorTable;

    class TerrainChunk
    {
    public:
        // TODO gfx::Material material;
        sf::VertexArray vertices;

        // TODO Rectangle bounds;

    };


private:
    // Data members
    sf::Color       _ambient_light;

    DescriptorTable _entity_descriptors;


public:
    // Use white to disable lighting
    const sf::Color & getAmbientLight() const { return _ambient_light; }

};
// class Map

}
// namespace game

#endif // _IWBAN_MAP_HPP_
