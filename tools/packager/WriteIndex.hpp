/**
 * @file   WriteIndex.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _PKG_WRITEINDEX_HPP_
#define _PKG_WRITEINDEX_HPP_

#include <packager/Index.hpp>

#include <ostream>

namespace pkg
{

namespace impl
{

inline void writeUInt32(std::ostream & stream, const uint32_t & value)
{
    stream.put(value);
    stream.put(value >> 8);
    stream.put(value >> 16);
    stream.put(value >> 24);
}

}
// namespace impl

void writeIndex(std::ostream & package, const IndexMap & index)
{
    uint32_t index_entries = index.size();
    impl::writeUInt32(package, index_entries);

    for (IndexMap::const_iterator it = index.begin();
            it != index.end(); ++it)
    {
        impl::writeUInt32(package, it->second.offset);
        impl::writeUInt32(package, it->second.size);

        const char * fname = it->first.c_str();
        package.write(fname, std::strlen(fname) + 1);
    }
}
// writeIndex()

}
// namespace pkg

#endif // _PKG_WRITEINDEX_HPP_
