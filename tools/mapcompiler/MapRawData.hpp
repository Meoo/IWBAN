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

    // Tile data[W * H];

    // Properties
    Properties properties;

    bool    solid;
    int     depth;

};
// class Layer

class Map
{
public:
    unsigned width;
    unsigned height;

    std::vector<Tileset> tilesets;
    TileDictionnary tiles;
    std::vector<Layer> layers;

    // Properties
    Properties properties;

    //Color light_color;

};
// class Map

#endif // _MAPC_MAPRAWDATA_HPP_
