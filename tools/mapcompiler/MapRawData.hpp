/**
 * @file   MapRawData.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _MAPC_MAPRAWDATA_HPP_
#define _MAPC_MAPRAWDATA_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

typedef std::map<std::string, std::string> Properties;

typedef uint32_t TileId;

class Tile;


class Tileset
{
public:
    // Types
    typedef std::vector<std::unique_ptr<Tile>> TileVector;

    // Data
    std::string name;

    // TODO Image image;

    unsigned first_gid;

    Properties properties;

    TileVector tiles;

};
// class Tileset


class Tile
{
public:
    // Data
    Tileset *   tileset;

    TileId      id;
    TileId      gid;

    Properties  properties;

};
// class Tile


class Layer
{
public:
    // Types
    typedef std::unique_ptr<Tile*[]> TileMap;

    // Data
    std::string name;

    TileMap     data;

    Properties  properties;

};
// class Layer


class Map
{
public:
    // Types
    typedef std::vector<std::unique_ptr<Tileset>>   TilesetVector;
    typedef std::map<TileId, Tile *>                TileDictionnary;
    typedef std::vector<std::unique_ptr<Layer>>     LayerVector;

    // Data
    unsigned        width;
    unsigned        height;

    TilesetVector   tilesets;
    TileDictionnary tiles;
    LayerVector     layers;

    Properties      properties;

};
// class Map

#endif // _MAPC_MAPRAWDATA_HPP_
