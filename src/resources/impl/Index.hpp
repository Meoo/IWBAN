/**
 * @file   Index.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INDEX_HPP_
#define _IWBAN_INDEX_HPP_

#include <Global.hpp>

#include <config/PackageConfig.hpp>

#include <utils/StreamIO.hpp>

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

inline
bool readIndex(std::istream & package, Index & index)
{
    if (ut::read<uint32_t>(package) != IWBAN_PKG_MAGIC)
        return false;

    uint32_t index_count = ut::read<uint32_t>(package);
    if (index_count > IWBAN_PKG_MAX_FILES)
        return false;

    for (uint32_t i = 0; i < index_count; ++i)
    {
        IndexEntry entry;

        entry.offset = ut::read<uint32_t>(package);
        entry.size = ut::read<uint32_t>(package);

        std::string filename = ut::read<std::string>(package);
        if (filename.empty())
            return false;

        index.insert({filename, entry});
    }

    return true;
}
// readIndex()

// ---- ---- ---- ----

inline
void writeIndex(std::ostream & package, const Index & index)
{
    ut::write<uint32_t>(package, IWBAN_PKG_MAGIC);

    uint32_t index_entries = index.size();
    ut::write<uint32_t>(package, index_entries);

    for (auto it = index.begin(); it != index.end(); ++it)
    {
        ut::write<uint32_t>(package, it->second.offset);
        ut::write<uint32_t>(package, it->second.size);
        ut::write<std::string>(package, it->first);
    }
}
// writeIndex()

}
// namespace impl

}
// namespace res

#endif // _IWBAN_INDEX_HPP_
