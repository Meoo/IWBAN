/**
 * @file   MapCompiler.cpp
 * @author Bastien Brunnenstein
 */

#include <mapcompiler/tinyxml/tinyxml2.h>

#include <iostream>
#include <map>
#include <string>

namespace tx = tinyxml2;

typedef std::map<std::string, std::string> Properties;

class Tileset
{
public:
    unsigned first_gid;

    // TODO Image image;

    Properties properties;

    std::map<unsigned, Tile> tiles;

};

class Tile
{
public:
    // TODO Tileset tileset;

    Properties properties;

};
// class Tile

typedef std::map<unsigned, Tile> TileDictionnary;

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


void print_help()
{
    std::cout << "===== IWBAN Map Compiler =====" << std::endl
              << "mapc file [files]...          - compile maps" << std::endl
              << "mapc -h                       - show this message" << std::endl;
}

// ---- ---- ---- ----

int process_file(const char * filename)
{
    tx::XMLDocument doc;
    tx::XMLError err = doc.LoadFile(filename);

    if (err != tx::XML_SUCCESS)
    {
        switch (err)
        {
        case tx::XML_ERROR_FILE_NOT_FOUND:
        case tx::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
        case tx::XML_ERROR_FILE_READ_ERROR:
            std::cerr << "!!!!! Could not open file " << filename
                      << " (" << err << ") !!!!!" << std::endl;
            break;

        default:
            std::cerr << "!!!!! Failed to parse file " << filename
                      << " (" << err << ") !!!!!" << std::endl;
            break;
        }
        return -1;
    }

    std::cout << "===== Processing file " << filename << " =====" << std::endl;

    std::cout << "+++ Parsing XML data +++" << std::endl;

    // Get root element
    tx::XMLElement * map = doc.FirstChildElement("map");

    if (map == 0)
    {
        std::cerr << "!!!!! XML file has no map root element !!!!!" << std::endl;
        return -1;
    }

    // Properties
    const char * version    = map->Attribute("version");
    if (!version || std::string(version) != "1.0")
    {
        std::cerr << "!!!!! TMX file format version is not good ("
                  << version << " !!!!!" << std::endl;
        return -1;
    }

    const char * width      = map->Attribute("width"); // in tiles
    const char * height     = map->Attribute("height"); // in tiles

    const char * tilewidth  = map->Attribute("tilewidth"); // in pixels
    const char * tileheight = map->Attribute("tileheight"); // in pixels

    Properties map_props;
    parse_properties(map, map_props);

    // Parse child elements
    for (tx::XMLElement * tileset = map->FirstChildElement("tileset");
            tileset != 0; tileset = tileset->NextSiblingElement("tileset"))
    {
        // Tileset
        std::cout << "TSET" << std::endl;

        const char * source = tileset->Attribute("");
        if (source)
        {
            std::cerr << "!!!!! External tilesets not supported !!!!!" << std::endl;
            return -1;
        }

    }

    for (tx::XMLElement * layer = map->FirstChildElement("layer");
            layer != 0; layer = layer->NextSiblingElement("layer"))
    {
        // Tiles layer
        std::cout << "LAYER" << std::endl;

        // Properties
        const char * layer_name = layer->Attribute("name");

        Properties layer_props;
        parse_properties(map, layer_props);

        tx::XMLElement * data = layer->FirstChildElement("data");
        if (!data)
        {
            std::cerr << "!!!!! No data node found in layer !!!!!" << std::endl;
            return -1;
        }

        if (data->Attribute("compression"))
        {
            std::cerr << "!!!!! Layer data compression not supported !!!!!" << std::endl;
            return -1;
        }

        if (data->Attribute("encoding", "csv"))
        {
            // TODO Parse CSV
        }
        else if (data->Attribute("encoding", "base64"))
        {
            // TODO Parse base64
        }
        else
        {
            std::cerr << "!!!!! Unsupported layer encoding !!!!!" << std::endl;
            return -1;
        }
    }

    for (tx::XMLElement * objlay = map->FirstChildElement("objectgroup");
            objlay != 0; objlay = objlay->NextSiblingElement("objectgroup"))
    {
        // Object layer
        std::cout << "OBJLAY" << std::endl;

        // Properties
        const char * objlay_name = objlay->Attribute("name");

        Properties objlay_props;
        parse_properties(objlay, objlay_props);

        for (tx::XMLElement * obj = objlay->FirstChildElement("object");
                obj != 0; obj = obj->NextSiblingElement("object"))
        {
            // Object
            std::cout << "OBJ" << std::endl;

            const char * obj_name = obj->Attribute("name");
            const char * obj_type = obj->Attribute("type");

            // Properties
            Properties obj_props;
            parse_properties(obj, obj_props);
        }
    }

    for (tx::XMLElement * img = map->FirstChildElement("imagelayer");
            img != 0; img = img->NextSiblingElement("imagelayer"))
    {
        // Images are ignored, issue a warning
        std::cout << "????? Image layers are not handled ?????" << std::endl;
    }

    // TODO XML parsed, process the data

    return 0;
}

// ---- ---- ---- ----

int main(int argc, char ** argv)
{
    if (argc == 1)
    {
        print_help();
        return 0;
    }

    if (argv[1][0] == '-')
    {
        switch(argv[1][1])
        {
        /* Stub if parameters are needed
        case 'x':
            if (argc != 4)
            {
                print_help();
                return 0;
            }
            return extract_file(argv[2], argv[3]);*/

        case 'h':
        default:
            print_help();
            return 0;
        }
    }

    // Default behavior : Compile maps
    for (int i = 1; i < argc; ++i)
    {
        int ret = process_file(argv[i]);

        if (ret != 0)
            return ret;
    }

    return 0;
}

