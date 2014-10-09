/**
 * @file   Packager.cpp
 * @author Bastien Brunnenstein
 */

#include <config/PackageConfig.hpp>

#include <resources/Index.hpp>

#include <boost/filesystem.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = boost::filesystem;
namespace pk = res;

typedef std::runtime_error Exception;

// ---- ---- ---- ----

void print_help()
{
    std::cout << "===== IWBAN Packager Tool =====" << std::endl
              << "pkgtool folder [folders]...   - package folders" << std::endl
              << "pkgtool -h                    - show this message" << std::endl
              << "pkgtool -l package            - list files in package" << std::endl
              << "pkgtool -x filename package   - extract file from package" << std::endl;
}

// ---- ---- ---- ----

void process_folder(const char * path_name)
{
    std::cout << "===== Processing " << path_name << " =====" << std::endl;

    // Check if given path is valid
    fs::path _path(path_name);

    if (!fs::exists(fs::path(_path)))
        throw Exception("Path " + std::string(path_name) + " is not valid");


    fs::path folder = fs::canonical(_path);

    // Path prefix length (+1 for separator)
    unsigned flen = std::strlen(folder.string().c_str()) + 1;

    if (!fs::is_directory(folder))
        throw Exception("File " + folder.string() + " is not a directory");


    // Path is a valid folder
    std::cout << "+++ Listing files +++" << std::endl;

    // Index size is header + entries + length of paths
    pk::Index index;
    std::size_t index_size = sizeof(pk::IndexHeader);

    // Recursively walk the folder
    for (fs::recursive_directory_iterator it(folder), end;
         it != end; ++it)
    {
        // Remove the path prefix to get the relative path
        const char * nice_path = it->path().generic_string().c_str() + flen;

        if (!fs::is_directory(it.status()))
        {
            // Increase index size (+1 is for '\0')
            index_size += sizeof(pk::IndexEntry) + std::strlen(nice_path) + 1;

            // Insert an entry in the map
            index.insert(pk::Index::value_type(nice_path, pk::IndexEntry()));

            // Check for maximum number of files in a package
            if (index.size() > IWBAN_PKG_MAX_FILES)
                throw Exception("Too many files in directory");
        }
    }

    // Open package file
    std::string package_name(folder.filename().string() + IWBAN_PKG_EXTENSION);

    std::ofstream package(package_name.c_str(),
            std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

    std::cout << "+++ Writing files +++" << std::endl;

    // ---- Write files

    // Reserve space for index
    package.seekp(index_size);

    // Iterate over all files
    for (auto it = index.begin(); it != index.end(); ++it)
    {
        // Append file to the package
        std::ifstream file((folder.string() + "/" + it->first).c_str(),
                std::ifstream::in | std::ifstream::binary);

        if (!file.is_open())
            throw Exception("Error while opening file " + it->first);

        std::size_t file_size;

        file.seekg(0, file.end);
        file_size = file.tellg();
        file.seekg(0, file.beg);

        // Go to next block if necessary
        std::size_t cpos = package.tellp();
        std::size_t pos = ((cpos - 1) / IWBAN_PKG_BLOCK_SIZE + 1) * IWBAN_PKG_BLOCK_SIZE;

        if ((pos - cpos) < file_size)
            package.seekp(pos);
        else
            pos = cpos;

        // Copy single file to package
        while (!file.eof())
        {
            static char buffer[IWBAN_PKG_BLOCK_SIZE];

            file.readsome(buffer, IWBAN_PKG_BLOCK_SIZE);
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
    pk::writeIndex(package, index);

    // ----

    std::cout << "+++ Package done +++" << std::endl;

    package.flush();
    package.close();

    std::cout << std::endl;

} // process_folder()

// ---- ---- ---- ----

void list_package(const char * packagefile)
{
    if (fs::is_directory(fs::path(packagefile)))
        throw Exception("Given path is a directory");

    std::ifstream package(packagefile,
            std::ifstream::in | std::ofstream::binary);

    if (!package.is_open())
        throw Exception("Error while opening package");

    pk::Index index;
    try
    {
        pk::readIndex(package, index);
    }
    catch (...)
    {
        // TODO Use readIndex exception instead?
        throw Exception("File is not a valid package");
    }

    for (auto it = index.begin(); it != index.end(); ++it)
    {
        std::cout << it->first << "\t: ";
        size_t sz = it->second.size;
        if (sz >= 0x00400000u)
        {
            std::cout << (it->second.size / 0x00100000u) << " MB" << std::endl;
        }
        else if (sz >= 0x00001000u)
        {
            std::cout << (it->second.size / 0x00000400u) << " KB" << std::endl;
        }
        else
        {
            std::cout << it->second.size << " B" << std::endl;
        }
    }

} // list_package()

// ---- ---- ---- ----

void extract_file(const char * file, const char * packagefile)
{
    if (fs::is_directory(fs::path(packagefile)))
        throw Exception("Given path is a directory");


    std::ifstream package(packagefile,
            std::ifstream::in | std::ofstream::binary);

    if (!package.is_open())
        throw Exception("Error while opening package");


    pk::Index index;
    try
    {
        pk::readIndex(package, index);
    }
    catch (...)
    {
        // TODO Use readIndex exception instead?
        throw Exception("File is not a valid package");
    }

    auto it = index.find(std::string(file));
    if (it == index.end())
        throw Exception("File not found in package");

    package.seekg(it->second.offset);

    std::ofstream out_file(fs::path(it->first).filename().string().c_str(),
            std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

    size_t size = it->second.size;
    while (size > 0)
    {
        char buffer[4096];

        package.read(buffer, (size < 4096 ? size : 4096));
        out_file.write(buffer, package.gcount());

        size -= package.gcount();
    }

} // extract_file()

// ---- ---- ---- ----

int main(int argc, char ** argv)
{
    try
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
            case 'l':
                // List
                if (argc != 3)
                    print_help();

                else
                    list_package(argv[2]);

                return 0;

            case 'x':
                // Extract file
                if (argc != 4)
                    print_help();

                else
                    extract_file(argv[2], argv[3]);

                return 0;

            case 'h':
            default:
                print_help();
                return 0;
            }
        }

        // Default behavior : Package folders
        for (int i = 1; i < argc; ++i)
            process_folder(argv[i]);

        return 0;
    }
    catch (const Exception & e)
    {
        std::cout << "!!! " << e.what() << " !!!" << std::endl;

        return 1;
    }
}

