/**
 * @file   MapCompiler.cpp
 * @author Bastien Brunnenstein
 */

#include "MapRawData.hpp"
#include "MapParser.hpp"
#include "TerrainBuilder.hpp"

#include "tinf/tinf.h"

#include <iostream>

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

    Map map;

    int ret = parse_map(filename, map);
    if (ret != 0)
        return ret;

    std::cout << "+++ Building terrain chunks +++" << std::endl;

    ret = build_terrain(map);
    if (ret != 0)
        return ret;

    std::cout << "+++ Building collision mask +++" << std::endl;

    // TODO Build collision mask

    std::cout << "+++ Building light and shadow volumes +++" << std::endl;

    // TODO Build light and shadow volumes

    std::cout << "+++ Writing file +++" << std::endl;

    // TODO Serialize map to file

    std::cout << "+++ Map done +++" << std::endl;

    return 0;
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

