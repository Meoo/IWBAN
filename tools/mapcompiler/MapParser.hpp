/**
 * @file   MapParser.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _MAPC_MAPPARSER_HPP_
#define _MAPC_MAPPARSER_HPP_

#include "Base64.hpp"
#include "MapRawData.hpp"

#include "tinf/tinf.h"
#include "tinyxml/tinyxml2.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <sstream>

namespace tx = tinyxml2;
namespace fs = boost::filesystem;

int open_document(const char * filename, tx::XMLDocument & document)
{
    tx::XMLError err = document.LoadFile(filename);
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
        return 1;
    }
    return 0;
}

// ---- ---- ---- ----

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
    if (open_document(filename, doc))
        return 1;

    // Get root element
    tx::XMLElement * map = doc.FirstChildElement("map");
    if (map == 0)
    {
        std::cerr << "!!! Map file has no map root element !!!" << std::endl;
        return 1;
    }

    // Properties
    const char * version = map->Attribute("version");
    if (!version || std::string(version) != "1.0")
    {
        std::cerr << "!!! TMX file format version is not good ("
                  << version << " !!!" << std::endl;
        return 1;
    }

    output_map.width    = map->UnsignedAttribute("width"); // in tiles
    output_map.height   = map->UnsignedAttribute("height"); // in tiles

    //unsigned tilewidth  = map->UnsignedAttribute("tilewidth"); // in pixels
    //unsigned tileheight = map->UnsignedAttribute("tileheight"); // in pixels

    parse_properties(map, output_map.properties);

    // ---- ---- ---- ----

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

            if (open_document(source_path.string().c_str(), tileset_doc))
                return 1;

            // Get root element
            tileset_source = tileset_doc.FirstChildElement("tileset");
            if (tileset_source == 0)
            {
                std::cerr << "!!! External tileset file has no tileset root element !!!" << std::endl;
                return 1;
            }
        }

        std::unique_ptr<Tileset> output_tileset(new Tileset());

        // Properties
        if (tileset_source->Attribute("name"))
            output_tileset->name = std::string(tileset_source->Attribute("name"));
        output_tileset->first_gid = tileset->UnsignedAttribute("gid");

        parse_properties(tileset_source, output_tileset->properties);

        for (tx::XMLElement * tile = tileset_source->FirstChildElement("tile");
                tile != 0; tile = tile->NextSiblingElement("tile"))
        {
            // Tile
std::cout << "TILE" << std::endl;

            std::unique_ptr<Tile> output_tile(new Tile());

            // Properties
            output_tile->tileset = output_tileset.get();
            output_tile->id = tile->UnsignedAttribute("id");
            output_tile->gid = output_tile->id + output_tileset->first_gid;
            parse_properties(tile, output_tile->properties);

            // Add tile to output data
            output_map.tiles[output_tile->gid] = output_tile.get();
            output_tileset->tiles.push_back(std::move(output_tile));
        }

        // Add tileset to output data
        output_map.tilesets.push_back(std::move(output_tileset));
    }

    // ---- ---- ---- ----

    for (tx::XMLElement * layer = map->FirstChildElement("layer");
            layer != 0; layer = layer->NextSiblingElement("layer"))
    {
        // Tiles layer
std::cout << "LAYER" << std::endl;

        std::unique_ptr<Layer> output_layer(new Layer());

        // Properties
        if (layer->Attribute("name"))
            output_layer->name = std::string(layer->Attribute("name"));
        parse_properties(layer, output_layer->properties);

        tx::XMLElement * data = layer->FirstChildElement("data");
        if (!data)
        {
            std::cerr << "!!! No data node found in layer !!!" << std::endl;
            return 1;
        }

        unsigned map_data_size = output_map.width * output_map.height;
        std::unique_ptr<TileId[]> map_data(new TileId[map_data_size]);

        if (data->Attribute("encoding", "csv"))
        {
            // Parse CSV
            std::istringstream csv_stream;
            csv_stream.str(std::string(data->GetText()));

            unsigned i = 0;
            while (csv_stream.good() && i < map_data_size)
            {
                // Read gid
                csv_stream >> map_data[i];

                // Ignore comma and skip spaces
                csv_stream.get();
                csv_stream >> std::ws;

                ++i;
            }

            if (i != map_data_size)
            {
                std::cerr << "!!! Not enough data in CSV-encoded layer !!!" << std::endl;
                return 1;
            }
        }
        else if (data->Attribute("encoding", "base64"))
        {
            // Get text and skip spaces
            const char * b64_text = data->GetText();
            while (b64_text[0] == ' ' || b64_text[0] == '\n' || b64_text[0] == '\t')
                ++b64_text;

            // Decode base64
            std::string raw_data = base64_decode(std::string(b64_text));

            if (data->Attribute("compression") != 0)
            {
                // Compressed data
                unsigned dest_len = map_data_size * sizeof(TileId);

                if (data->Attribute("compression", "zlib"))
                {
                    // TODO Endianness in map_data is system dependant, raw_data is little-endian
                    if (tinf_zlib_uncompress(map_data.get(), &dest_len, raw_data.c_str(), raw_data.length()) == TINF_DATA_ERROR)
                    {
                        std::cerr << "!!! Zlib uncompression failed !!!" << std::endl;
                        return 1;
                    }
                }
                else if (data->Attribute("compression", "gzip"))
                {
                    // TODO Endianness in map_data is system dependant, raw_data is little-endian
                    if (tinf_gzip_uncompress(map_data.get(), &dest_len, raw_data.c_str(), raw_data.length()) == TINF_DATA_ERROR)
                    {
                        std::cerr << "!!! Gzip uncompression failed !!!" << std::endl;
                        return 1;
                    }
                }
                else
                {
                    std::cerr << "!!! Unsupported compression method !!!" << std::endl;
                    return -1;
                }
            }
            else
            {
                // Uncompressed data
                // TODO Endianness in map_data is system dependant, raw_data is little-endian
                std::memcpy(map_data.get(), raw_data.c_str(), raw_data.length());
            }

            // TODO Use data
            /*for (unsigned i = 0; i < map_data_size; ++i)
            {
                std::cerr << map_data[i] << ' ';
                if (i % output_map.width == 0)
                    std::cerr << std::endl;
            }*/
        }
        else
        {
            std::cerr << "!!! Unsupported layer encoding !!!" << std::endl;
            return -1;
        }

        // Add layer to output data
        output_map.layers.push_back(std::move(output_layer));
    }

    // ---- ---- ---- ----

    for (tx::XMLElement * objlay = map->FirstChildElement("objectgroup");
            objlay != 0; objlay = objlay->NextSiblingElement("objectgroup"))
    {
        // Object layer
std::cout << "OBJLAY" << std::endl;

        // Properties
        //const char * objlay_name = objlay->Attribute("name");

        //parse_properties(objlay, objlay_props);

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

    // ---- ---- ---- ----

    for (tx::XMLElement * img = map->FirstChildElement("imagelayer");
            img != 0; img = img->NextSiblingElement("imagelayer"))
    {
        // Images are ignored, issue a warning
        std::cout << "??? Image layers are not handled ???" << std::endl;
    }

    return 0;
}

#endif // _MAPC_MAPPARSER_HPP_
