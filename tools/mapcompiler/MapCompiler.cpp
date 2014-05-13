/**
 * @file   MapCompiler.cpp
 * @author Bastien Brunnenstein
 */

#include <mapcompiler/tinyxml/tinyxml2.h>

#include <iostream>
#include <string>

namespace tx = tinyxml2;

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

    // Get root element
    tx::XMLElement * map = doc.FirstChildElement("map");

    if (map == 0)
    {
        std::cerr << "!!!!! XML file has no map root element !!!!!" << std::endl;
        return -1;
    }

    for (tx::XMLElement * props = map->FirstChildElement("properties");
            props != 0; props = props->NextSiblingElement("properties"))
    {
        // Properties
        for (tx::XMLElement * prop = map->FirstChildElement("property");
            prop != 0; prop = prop->NextSiblingElement("property"))
        {
            // Property
            std::cout << "PROP" << std::endl;
        }
    }

    for (tx::XMLElement * tileset = map->FirstChildElement("tileset");
            tileset != 0; tileset = tileset->NextSiblingElement("tileset"))
    {
        // Tilesets
        std::cout << "TSET" << std::endl;
    }

    for (tx::XMLElement * layer = map->FirstChildElement("layer");
            layer != 0; layer = layer->NextSiblingElement("layer"))
    {
        // Layers
        std::cout << "LAYER" << std::endl;
    }

    for (tx::XMLElement * objs = map->FirstChildElement("objectgroup");
            objs != 0; objs = objs->NextSiblingElement("objectgroup"))
    {
        // Objects
        std::cout << "OBJS" << std::endl;
    }

    for (tx::XMLElement * img = map->FirstChildElement("imagelayer");
            img != 0; img = img->NextSiblingElement("imagelayer"))
    {
        // Images
        std::cout << "????? Image layers are not handled ?????" << std::endl;
    }

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

    // Default behavior : Package folders
    for (int i = 1; i < argc; ++i)
    {
        int ret = process_file(argv[i]);

        if (ret != 0)
            return ret;
    }

    return 0;
}

