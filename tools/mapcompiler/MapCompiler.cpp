/**
 * @file   MapCompiler.cpp
 * @author Bastien Brunnenstein
 */

#include "InputMap.hpp"
#include "MapParser.hpp"
#include "ProcessLayer.hpp"

#include "tinf/tinf.h"

#include <config/MapConfig.hpp>

#include <utils/StreamIO.hpp>

#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>

namespace fs = boost::filesystem;

void print_help()
{
    std::cout << "===== IWBAN Map Compiler =====" << std::endl
              << "mapc file [files]...          - compile maps" << std::endl
              << "mapc -h                       - show this message" << std::endl;
}

// ---- ---- ---- ----

int process_file(const char * filename)
{
    std::cout << "===== Processing " << filename << " =====" << std::endl;

    std::cout << "+++ Parsing XML data +++" << std::endl;

    InputMap map;
    int ret;

    ret = parse_map(filename, map);
    if (ret != 0)
        return 1;


    std::cout << "+++ Opening output file +++" << std::endl;

    // Replace extension
    fs::path out_path(filename);
    out_path = out_path.parent_path() / out_path.stem();
    out_path += IWBAN_MAP_EXTENSION;

    // Open file
    std::ofstream out_map(out_path.string(), std::ofstream::binary | std::ofstream::trunc);


    // Write header
    ut::write<uint32_t>(out_map, IWBAN_MAP_MAGIC);
    ut::write<uint32_t>(out_map, IWBAN_MAP_VERSION);


    // Texture table
    ut::write<uint32_t>(out_map, map.texture_table.size());
    for (const std::string & texture : map.texture_table)
        ut::write<std::string>(out_map, texture);


    // Layers
    ut::write<uint32_t>(out_map, map.layers.size());
    for (const std::unique_ptr<Layer> & layer : map.layers)
    {
        std::cout << "+++ Building layer " << layer->name << " +++" << std::endl;

        ret = process_layer(map, *layer.get(), out_map);
        if (ret != 0)
            goto process_failed;
    }


    // TODO Everything else
    // ...


    std::cout << "+++ Map done +++" << std::endl;

    // Flush and close map file
    out_map.flush();
    out_map.close();
    return 0;

process_failed:
    // Close and delete failed map file
    out_map.close();
    fs::remove(out_path);
    return 1;
}

// ---- ---- ---- ----

int main(int argc, char ** argv)
{
    tinf_init();

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

