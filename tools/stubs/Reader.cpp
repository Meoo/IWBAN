
#include <fstream>
#include <iostream>
#include <string>

#include <boost/container/map.hpp>
#include <boost/filesystem.hpp>


// TODO This file should be removed at one point. At least do not compile it

namespace fs = boost::filesystem;


class IndexEntry
{
public:
    uint32_t   offset;
    uint32_t   size;
};
typedef boost::container::map<std::string, IndexEntry> IndexMap;


void read_index(const std::string & filename)
{
    std::ifstream package(filename.c_str(),
            std::ifstream::in | std::ifstream::binary);

    std::size_t file_size;

    package.seekg(0, package.end);
    file_size = package.tellg();
    package.seekg(0, package.beg);

    uint32_t index_count;
    // TODO Endianness
    package.read((char*) &index_count, sizeof(uint32_t));

    // TODO Check index count value?
    if (index_count > GRAND_MAX)
    {
        // TODO Error, index count is invalid, file is corrupted
        // Throw exception
    }

    IndexMap index;
    index.reserve(index_count);

    for (uint32_t i = 0; i < index_count; ++i)
    {
        IndexEntry entry;

        // TODO Endianness
        package.read((char*) &(entry.offset), sizeof(uint32_t));
        package.read((char*) &(entry.size), sizeof(uint32_t));

        // Check values
        if (entry.offset + entry.size > file_size)
        {
            // TODO Error, entry is out of bounds, file is corrupted
            // Throw exception
        }

        char filename[512];
        package.getline(filename, sizeof(filename), '\0');

        if (package.fail() || filename[0] == 0)
        {
            // TODO Error, filename is not valid, file is corrupted 
            // Throw exception
        }

        index.insert(IndexMap::value_type(filename, entry));
    }

    package.close();
}

