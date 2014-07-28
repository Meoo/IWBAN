/**
 * @file   MapRawData.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _MAPC_MAPRAWDATA_HPP_
#define _MAPC_MAPRAWDATA_HPP_

#include <map>
#include <string>
#include <vector>

typedef std::map<std::string, std::string> Properties;

class Tile;

class Tileset

{
public:
    std::string name;

    // TODO Image image;

    Properties properties;

    unsigned first_gid;

};
// class Tileset

class Tile
{
public:
    // TODO Tileset tileset;

    Properties properties;

    unsigned id;
    unsigned gid;

};
// class Tile

typedef std::map<unsigned, Tile> TileDictionnary;

class Layer
{
public:
    std::string name;

    Properties properties;

    // Tile data[W * H];

};
// class Layer

class Map
{
public:
    Properties properties;

    unsigned width;
    unsigned height;

    std::vector<Tileset> tilesets;
    TileDictionnary tiles;
    std::vector<Layer> layers;

};
// class Map

#endif // _MAPC_MAPRAWDATA_HPP_
