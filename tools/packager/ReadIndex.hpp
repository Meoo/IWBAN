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

// TODO Can probably be optimized
uint32_t readUInt32(std::istream & stream)
{
    uint32_t r = static_cast<unsigned char>(stream.get()) << 24;
    r |= static_cast<unsigned char>(stream.get()) << 16;
    r |= static_cast<unsigned char>(stream.get()) << 8;
    r |= static_cast<unsigned char>(stream.get());

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

    if (impl::readUInt32(package) != PKG_MAGIC)
        return false;

    uint32_t index_count = impl::readUInt32(package);
    if (index_count > PKG_MAX_FILES)
        return false;

    for (uint32_t i = 0; i < index_count; ++i)
    {
        IndexEntry entry;

        entry.offset = impl::readUInt32(package);
        entry.size = impl::readUInt32(package);

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
