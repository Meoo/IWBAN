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

void writeIndex(std::ostream & package, const IndexMap & index)
{
    // TODO Endianness
    uint32_t index_entries = index.size();
    package.write((const char *) &index_entries, sizeof(uint32_t));

    for (IndexMap::const_iterator it = index.begin();
            it != index.end(); ++it)
    {
        // TODO Endianness
        package.write((const char *) &(it->second.offset), sizeof(uint32_t));
        package.write((const char *) &(it->second.size), sizeof(uint32_t));

        const char * fname = it->first.c_str();
        package.write(fname, std::strlen(fname) + 1);
    }
}
// writeIndex()

}
// namespace pkg

#endif // _PKG_WRITEINDEX_HPP_
