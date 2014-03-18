/**
 * @file   ReadIndex.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _PKG_READINDEX_HPP_
#define _PKG_READINDEX_HPP_

#include <packager/PackagerConfig.hpp>
#include <packager/Index.hpp>

#include <istream>

namespace pkg
{

namespace impl
{

uint32_t readUInt32(std::istream & stream)
{
    char c;
    uint32_t r;

    stream.get(c);
    r = c;
    
    stream.get(c);
    r &= c << 8;
    
    stream.get(c);
    r &= c << 16;
    
    stream.get(c);
    r &= c << 24;
    
    return r;
}

}
// namespace impl

bool readIndex(std::istream & package, IndexMap & index)
{
    std::size_t file_size;

    package.seekg(0, package.end);
    file_size = package.tellg();
    package.seekg(0, package.beg);

    uint32_t index_count;
    // TODO Endianness
    package.read((char*) &index_count, sizeof(uint32_t));

    // TODO Check index count value?
    if (index_count > PKG_MAX_FILES)
        return false;

    for (uint32_t i = 0; i < index_count; ++i)
    {
        IndexEntry entry;

        // TODO Endianness
        package.read((char*) &(entry.offset), sizeof(uint32_t));
        package.read((char*) &(entry.size), sizeof(uint32_t));

        // Check values
        if (entry.offset + entry.size > file_size)
            return false;

        char filename[512];
        package.getline(filename, sizeof(filename), '\0');

        if (package.fail() || filename[0] == 0)
            return false;

        index.insert(IndexMap::value_type(filename, entry));
    }

    return true;
}
// readIndex()

}
// namespace pkg

#endif // _PKG_READINDEX_HPP_
