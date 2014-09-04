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
    typedef std::unique_ptr<Tile[]> TileArray;

    // Data
    std::string name;

    unsigned    texture_id;

    unsigned    first_gid;

    unsigned    tile_width;
    unsigned    tile_height;

    unsigned    tile_rows;
    unsigned    tile_lines;

    unsigned    spacing;
    unsigned    margin;

    unsigned    offset_x;
    unsigned    offset_y;

    Properties  properties;

    TileArray   tiles;

};
// class Tileset


class Tile
{
public:
    // Data
    Tileset *   tileset;

    TileId      id;

    unsigned    u;  // Left border coord in image
    unsigned    v;  // Top border coord in image

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


class InputMap
{
public:
    // Types
    typedef std::vector<std::string>                TextureTable;
    typedef std::vector<std::unique_ptr<Tileset>>   TilesetVector;
    typedef std::map<TileId, Tile *>                TileDictionnary;
    typedef std::vector<std::unique_ptr<Layer>>     LayerVector;

    // Data
    unsigned        width;
    unsigned        height;

    unsigned        tile_width;
    unsigned        tile_height;

    TextureTable    texture_table;
    TilesetVector   tilesets;
    TileDictionnary tiles;
    LayerVector     layers;

    Properties      properties;

};
// class Map

#endif // _MAPC_MAPRAWDATA_HPP_
