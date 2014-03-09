/**
 * @file   Packager.cpp
 * @author Bastien Brunnenstein
 */

#include <packager/PackagerConfig.hpp>
#include <packager/WriteIndex.hpp>

#include <boost/filesystem.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = boost::filesystem;

void process_folder(const char * path_name)
{


    using pkg::IndexMap;
    using pkg::IndexEntry;

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

            // Check for maximum number of files in a package
            if (index.size() > PKG_MAX_FILES)
            {
                std::cout << "!!! Too many files in directory !!!" << std::endl;
                return;
            }
        }
    }

    // Open package file
    std::string package_name(folder.leaf().string() + PKG_EXTENSION);

    std::ofstream package(package_name.c_str(),
            std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

    std::cout << "+++ Writing files +++" << std::endl;

    // ---- Write files

    // Reserve space for index
    package.seekp(index_size);

    // Iterate over all files
    for (IndexMap::iterator it = index.begin();
            it != index.end(); ++it)
    {
        // Append file to the package
        std::ifstream file((folder.string() + "/" + it->first).c_str(),
                std::ifstream::in | std::ifstream::binary);

        std::size_t file_size;

        file.seekg(0, file.end);
        file_size = file.tellg();
        file.seekg(0, file.beg);

        if (!file.is_open())
        {
            std::cout << "!!! Error while opening file " << it->first.c_str()
                      << " !!!" << std::endl;
            return;
        }

        // Go to next block if necessary
        std::size_t cpos = package.tellp();
        std::size_t pos = ((cpos - 1) / PKG_BLOCK_SIZE + 1) * PKG_BLOCK_SIZE;

        std::cout << (pos - cpos) << " < " << file_size << std::endl;
        if ((pos - cpos) < file_size)
            package.seekp(pos);
        else
            pos = cpos;

        // Copy single file to package
        while (!file.eof())
        {
            static char buffer[PKG_BLOCK_SIZE];

            file.readsome(buffer, PKG_BLOCK_SIZE);
            package.write(buffer, file.gcount());

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
    pkg::writeIndex(package, index);

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

