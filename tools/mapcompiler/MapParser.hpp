/**
 * @file   MapParser.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _MAPC_MAPPARSER_HPP_
#define _MAPC_MAPPARSER_HPP_

#include "MapRawData.hpp"

#include <mapcompiler/tinyxml/tinyxml2.h>

#include <boost/filesystem.hpp>

#include <iostream>

namespace tx = tinyxml2;
namespace fs = boost::filesystem;

// Find <properties> in element, and parse all <property> from it
void parse_properties(const tx::XMLElement * element, Properties & properties)
{
    for (const tx::XMLElement * props = element->FirstChildElement("properties");
            props != 0; props = props->NextSiblingElement("properties"))
    {
        // Properties
        for (const tx::XMLElement * prop = props->FirstChildElement("property");
            prop != 0; prop = prop->NextSiblingElement("property"))
        {
            // Property
            const char * key    = prop->Attribute("name");
            const char * value  = prop->Attribute("value");

            if (!key || value)
            {
                std::cout << "????? Missing key or value attribute from"
                          << " property element, ignored ?????" << std::endl;
                continue;
            }

            properties[std::string(key)] = std::string(value);
        }
    }
}

// ---- ---- ---- ----

int parse_map(const char * filename, Map & output_map)
{
    tx::XMLDocument doc;

    // Enclose err in a local scope
    {
        tx::XMLError err = doc.LoadFile(filename);
        if (err != tx::XML_SUCCESS)
        {
            switch (err)
            {
            case tx::XML_ERROR_FILE_NOT_FOUND:
            case tx::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
            case tx::XML_ERROR_FILE_READ_ERROR:
                std::cerr << "!!! Could not open file " << filename
                          << " (" << err << ") !!!" << std::endl;
                break;

            default:
                std::cerr << "!!! Failed to parse file " << filename
                          << " (" << err << ") !!!" << std::endl;
                break;
            }
            return -1;
        }
    }

    // Get root element
    tx::XMLElement * map = doc.FirstChildElement("map");

    if (map == 0)
    {
        std::cerr << "!!! Map file has no map root element !!!" << std::endl;
        return 1;
    }

    // Properties
    const char * version    = map->Attribute("version");
    if (!version || std::string(version) != "1.0")
    {
        std::cerr << "!!! TMX file format version is not good ("
                  << version << " !!!" << std::endl;
        return 1;
    }

    //unsigned width      = map->UnsignedAttribute("width"); // in tiles
    //unsigned height     = map->UnsignedAttribute("height"); // in tiles

    //unsigned tilewidth  = map->UnsignedAttribute("tilewidth"); // in pixels
    //unsigned tileheight = map->UnsignedAttribute("tileheight"); // in pixels

    Properties map_props;
    parse_properties(map, map_props);

    // Parse child elements
    for (tx::XMLElement * tileset = map->FirstChildElement("tileset");
            tileset != 0; tileset = tileset->NextSiblingElement("tileset"))
    {
        // Tileset
std::cout << "TILESET" << std::endl;

        tx::XMLDocument tileset_doc; // Only used if an external tileset is used
        tx::XMLElement * tileset_source = tileset;

        // External tileset
        const char * source_filename = tileset->Attribute("source");
        if (source_filename)
        {
            // source_filename is a relative path
            // Append source_filename to parent path of filename
            fs::path source_path = fs::path(filename).parent_path();
            source_path /= fs::path(source_filename);

            tx::XMLError err = tileset_doc.LoadFile(source_path.string().c_str());
            if (err != tx::XML_SUCCESS)
            {
                switch (err)
                {
                case tx::XML_ERROR_FILE_NOT_FOUND:
                case tx::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
                case tx::XML_ERROR_FILE_READ_ERROR:
                    std::cerr << "!!! Could not open external tileset " << source_filename
                              << " (" << err << ") !!!" << std::endl;
                    break;

                default:
                    std::cerr << "!!! Failed to parse external tileset " << source_filename
                              << " (" << err << ") !!!" << std::endl;
                    break;
                }
                return -1;
            }

            // Get root element
            tileset_source = tileset_doc.FirstChildElement("tileset");

            if (tileset_source == 0)
            {
                std::cerr << "!!! External tileset file has no tileset root element !!!" << std::endl;
                return 1;
            }
        }

        // Properties
        //const char * tileset_name = tileset->Attribute("name");
        //unsigned tileset_gid = tileset->UnsignedAttribute("gid");
        Properties tileset_props;
        parse_properties(tileset_source, tileset_props);

        for (tx::XMLElement * tile = tileset_source->FirstChildElement("tile");
                tile != 0; tile = tile->NextSiblingElement("tile"))
        {
            // Tile
std::cout << "TILE" << std::endl;

            // Properties
            //unsigned tile_id = tile->UnsignedAttribute("id");
            //unsigned tile_gid = tile_id + tileset_gid;
            Properties tile_props;
            parse_properties(tile, tile_props);
        }
    }

    for (tx::XMLElement * layer = map->FirstChildElement("layer");
            layer != 0; layer = layer->NextSiblingElement("layer"))
    {
        // Tiles layer
std::cout << "LAYER" << std::endl;

        // Properties
        //const char * layer_name = layer->Attribute("name");
        Properties layer_props;
        parse_properties(layer, layer_props);

        tx::XMLElement * data = layer->FirstChildElement("data");
        if (!data)
        {
            std::cerr << "!!! No data node found in layer !!!" << std::endl;
            return -1;
        }

        if (data->Attribute("compression") != 0)
        {
            std::cerr << "!!! Layer data compression not supported !!!" << std::endl;
            return -1;
        }

        if (data->Attribute("encoding", "csv"))
        {
            // TODO Parse CSV
        }
        /*else if (data->Attribute("encoding", "base64"))
        {
            // TODO Parse base64
        }*/
        else
        {
            std::cerr << "!!! Unsupported layer encoding, use CSV !!!" << std::endl;
            return -1;
        }
    }

    for (tx::XMLElement * objlay = map->FirstChildElement("objectgroup");
            objlay != 0; objlay = objlay->NextSiblingElement("objectgroup"))
    {
        // Object layer
std::cout << "OBJLAY" << std::endl;

        // Properties
        //const char * objlay_name = objlay->Attribute("name");

        Properties objlay_props;
        parse_properties(objlay, objlay_props);

        for (tx::XMLElement * obj = objlay->FirstChildElement("object");
                obj != 0; obj = obj->NextSiblingElement("object"))
        {
            // Object
std::cout << "OBJ" << std::endl;

            //const char * obj_name = obj->Attribute("name");
            //const char * obj_type = obj->Attribute("type");

            // Properties
            Properties obj_props;
            parse_properties(obj, obj_props);
        }
    }

    for (tx::XMLElement * img = map->FirstChildElement("imagelayer");
            img != 0; img = img->NextSiblingElement("imagelayer"))
    {
        // Images are ignored, issue a warning
        std::cout << "??? Image layers are not handled ???" << std::endl;
    }

    return 0;
}

#endif // _MAPC_MAPPARSER_HPP_
