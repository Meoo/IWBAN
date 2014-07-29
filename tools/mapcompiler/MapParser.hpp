/**
 * @file   MapParser.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _MAPC_MAPPARSER_HPP_
#define _MAPC_MAPPARSER_HPP_

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
        if (tileset->Attribute("name"))
            output_tileset->name = std::string(tileset->Attribute("name"));
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

        if (data->Attribute("encoding", "csv"))
        {
            // Parse CSV
            std::istringstream csv_stream;
            csv_stream.str(std::string(data->GetText()));

            while (csv_stream.good())
            {
                TileId gid;

                // Read gid
                csv_stream >> gid;

                // TODO Use data

                // Ignore comma and skip spaces
                csv_stream.get();
                csv_stream >> std::ws;
            }
        }
        else if (data->Attribute("encoding", "base64"))
        {
            // TODO Parse base64
            const char * b64_data = data->GetText();
            unsigned b64_len = std::strlen(b64_data);

            unsigned raw_data_len = (b64_len / 4) * 3;
            if (b64_data[b64_len - 1] == '=') --raw_data_len;
            if (b64_data[b64_len - 2] == '=') --raw_data_len;

            std::unique_ptr<unsigned char[]> raw_data(new unsigned char[raw_data_len]);
            // TODO Parse base64


            if (data->Attribute("compression") != 0)
            {
                // Compressed data
                unsigned dest_len = output_map.width * output_map.height * sizeof(TileId);
                std::unique_ptr<unsigned char[]> dest(new unsigned char[dest_len]);

                if (data->Attribute("compression", "zlib"))
                {
                    if (tinf_zlib_uncompress(dest.get(), &dest_len, raw_data.get(), raw_data_len) == TINF_DATA_ERROR)
                    {
                        std::cerr << "!!! Zlib uncompression failed !!!" << std::endl;
                        return 1;
                    }
                }
                else if (data->Attribute("compression", "gzip"))
                {
                    if (tinf_gzip_uncompress(dest.get(), &dest_len, raw_data.get(), raw_data_len) == TINF_DATA_ERROR)
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

                raw_data = std::move(dest);
            }

            // TODO Use data
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
