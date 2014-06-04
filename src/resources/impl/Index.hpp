/**
 * @file   Index.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INDEX_HPP_
#define _IWBAN_INDEX_HPP_

#include <Global.hpp>

#include <config/PackageConfig.hpp>

#include <iostream>
#include <map>

namespace res
{

namespace impl
{

class IndexEntry
{
public:
    uint32_t   offset;
    uint32_t   size;
};

typedef std::map<std::string, IndexEntry> Index;

// ---- ---- ---- ----

// TODO Replace with upcoming stream IO utils
inline
uint32_t readUInt32(std::istream & stream)
{
    uint32_t r = static_cast<unsigned char>(stream.get()) << 24;
    r |= static_cast<unsigned char>(stream.get()) << 16;
    r |= static_cast<unsigned char>(stream.get()) << 8;
    r |= static_cast<unsigned char>(stream.get());

    return r;
}

inline
bool readIndex(std::istream & package, Index & index)
{
    std::size_t file_size;

    package.seekg(0, package.end);
    file_size = package.tellg();
    package.seekg(0, package.beg);

    if (readUInt32(package) != IWBAN_PKG_MAGIC)
        return false;

    uint32_t index_count = readUInt32(package);
    if (index_count > IWBAN_PKG_MAX_FILES)
        return false;

    for (uint32_t i = 0; i < index_count; ++i)
    {
        IndexEntry entry;

        entry.offset = readUInt32(package);
        entry.size = readUInt32(package);

        // Check values
        if (entry.offset + entry.size > file_size)
            return false;

        char filename[512];
        package.getline(filename, sizeof(filename), '\0');

        if (package.fail() || filename[0] == 0)
            return false;

        index.insert({filename, entry});
    }

    return true;
}
// readIndex()

// ---- ---- ---- ----

// TODO Replace with upcoming stream IO utils
inline
void writeUInt32(std::ostream & stream, const uint32_t & value)
{
    stream.put(value >> 24);
    stream.put(value >> 16);
    stream.put(value >> 8);
    stream.put(value);
}

inline
void writeIndex(std::ostream & package, const Index & index)
{
    writeUInt32(package, IWBAN_PKG_MAGIC);

    uint32_t index_entries = index.size();
    writeUInt32(package, index_entries);

    for (auto it = index.begin(); it != index.end(); ++it)
    {
        writeUInt32(package, it->second.offset);
        writeUInt32(package, it->second.size);

        const char * fname = it->first.c_str();
        package.write(fname, std::strlen(fname) + 1);
    }
}
// writeIndex()

}
// namespace impl

}
// namespace res

#endif // _IWBAN_INDEX_HPP_
