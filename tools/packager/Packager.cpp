/**
 * @file   Packager.cpp
 * @author Bastien Brunnenstein
 */

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include <boost/container/map.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

// TODO Put config macros in a shared file?

#define PACKAGE_EXTENSION ".dp"

// Block size is 4k
#define BLOCK_SIZE  0x00001000

// TODO Better uint32_t
typedef unsigned int uint32_t;

class IndexEntry
{
public:
    uint32_t   offset;
    uint32_t   size;
};
typedef boost::container::map<std::string, IndexEntry> IndexMap;

void process_folder(const char * path_name)
{
    // Check if given path is valid
    fs::path _path(path_name);

    if (!fs::exists(fs::path(_path)))
    {
        std::cout << "!!!!! Path " << path_name << " is not valid !!!!!"
                  << std::endl << std::endl;
        return;
    }

    fs::path folder = fs::canonical(_path);

    // Path prefix length (+1 for separator)
    unsigned flen = std::strlen(folder.string().c_str()) + 1;

    if (!fs::is_directory(folder))
    {
        std::cout << "!!!!! File " << folder.string().c_str()
                  << " is not a directory !!!!!" << std::endl << std::endl;
        return;
    }

    // Path is a valid folder

    std::cout << "===== Processing directory " << folder.leaf().string().c_str()
              << " =====" << std::endl;

    // Index size is number of entries + length of paths + size of IndexEntries
    IndexMap index;
    std::size_t index_size = sizeof(uint32_t);

    std::cout << "+++ Listing files +++" << std::endl;

    // Recursively walk the folder
    for (fs::recursive_directory_iterator it(folder), end;
         it != end; ++it)
    {
        // Remove the path prefix to get the relative path
        const char * nice_path = it->path().string().c_str() + flen;

        if (!fs::is_directory(it.status()))
        {
            // Increase index size (+1 is for '\0')
            index_size += sizeof(IndexEntry) + std::strlen(nice_path) + 1;

            // Insert an entry in the map
            index.insert(IndexMap::value_type(nice_path, IndexEntry()));
        }
    }

    // Open package file
    std::string package_name(folder.leaf().string() + PACKAGE_EXTENSION);

    std::ofstream package(package_name.c_str(),
            std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

    std::cout << "+++ Writing files +++" << std::endl;

    // ---- Write files

    // Reserve space for index
    package.seekp(index_size + 1);

    // Iterate over all files
    for (IndexMap::iterator it = index.begin();
            it != index.end(); ++it)
    {
        // Append file to the package
        std::ifstream file((folder.string() + "/" + it->first).c_str(),
                std::ifstream::in | std::ifstream::binary);

        if (!file.is_open())
        {
            std::cout << "!!! Error while opening file " << it->first.c_str()
                      << " !!!" << std::endl;
            return;
        }

        // Go to next block
        std::size_t pos = ((std::size_t(package.tellp()) - 1) / BLOCK_SIZE + 1) * BLOCK_SIZE;
        package.seekp(pos);

        // Copy single file to package
        int file_size = 0;
        while (!file.eof())
        {
            static char buffer[BLOCK_SIZE];

            file.readsome(buffer, BLOCK_SIZE);
            package.write(buffer, file.gcount());

            file_size += file.gcount();

            if (file.gcount() == 0) break;
        }

        file.close();

        // Fill IndexEntry
        it->second.offset = pos; // Offset is absolute, not in blocks
        it->second.size = file_size;
    }

    // ----

    std::cout << "+++ Writing package index +++" << std::endl;

    // ---- Write index

    package.seekp(0);

    // TODO Endianness
    uint32_t index_entries = index.size();
    package.write((const char *) &index_entries, sizeof(uint32_t));

    for (IndexMap::iterator it = index.begin();
            it != index.end(); ++it)
    {
        // TODO Endianness
        package.write((const char *) &(it->second.offset), sizeof(uint32_t));
        package.write((const char *) &(it->second.size), sizeof(uint32_t));

        const char * fname = it->first.c_str();
        package.write(fname, std::strlen(fname) + 1);
    }

    // ----

    std::cout << "+++ Package done +++" << std::endl;

    package.flush();
    package.close();

    std::cout << std::endl;
}


int main(int argc, char ** argv)
{
    for (int i = 1; i < argc; ++i)
        process_folder(argv[i]);

    return 0;
}

