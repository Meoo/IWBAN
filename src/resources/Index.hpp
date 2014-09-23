/**
 * @file   Index.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INDEX_HPP_
#define _IWBAN_INDEX_HPP_

#include <Global.hpp>

#include <config/PackageConfig.hpp>

#include <utils/StreamIO.hpp>

#include <system/exceptions/DataCorrupted.hpp>

#include <iostream>
#include <map>

namespace res
{

class IndexHeader
{
public:
    uint32_t    magic;
    uint8_t     version;
    uint16_t    file_count;

    void read(std::istream & istr)
    {
         ut::read(istr, magic);
         ut::read(istr, version);
         ut::read(istr, file_count);
    }

    void write(std::ostream & ostr) const
    {
         ut::write(ostr, magic);
         ut::write(ostr, version);
         ut::write(ostr, file_count);
    }
};

// ---- ---- ---- ----

class IndexEntry
{
public:
    uint32_t   offset;
    uint32_t   size;

    void read(std::istream & istr)
    {
         ut::read(istr, offset);
         ut::read(istr, size);
    }

    void write(std::ostream & ostr) const
    {
         ut::write(ostr, offset);
         ut::write(ostr, size);
    }
};

typedef std::map<std::string, IndexEntry> Index;

// ---- ---- ---- ----

inline
void readIndex(std::istream & package, Index & index)
{
    IndexHeader header;

    header.read(package);

    if (header.magic != IWBAN_PKG_MAGIC)
        throw sys::DataCorrupted("Invalid magic");

    if (header.version != IWBAN_PKG_VERSION)
        throw sys::DataCorrupted("Invalid version");

    if (header.file_count > IWBAN_PKG_MAX_FILES)
        throw sys::DataCorrupted("Too many files");

    for (unsigned i = 0; i < header.file_count; ++i)
    {
        IndexEntry entry;

        entry.read(package);

        std::string filename = ut::read<std::string>(package);
        if (filename.empty())
            throw sys::DataCorrupted("File name is empty");

        index.insert({filename, entry});
    }
}
// readIndex()

// ---- ---- ---- ----

inline
void writeIndex(std::ostream & package, const Index & index)
{
    IndexHeader header;

    header.magic = IWBAN_PKG_MAGIC;
    header.version = IWBAN_PKG_VERSION;
    header.file_count = index.size();

    header.write(package);

    for (auto it = index.begin(); it != index.end(); ++it)
    {
        it->second.write(package);
        ut::write<std::string>(package, it->first);
    }
}
// writeIndex()

}
// namespace res

#endif // _IWBAN_INDEX_HPP_
