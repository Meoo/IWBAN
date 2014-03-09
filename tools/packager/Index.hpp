/**
 * @file   Index.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _PKG_INDEX_HPP_
#define _PKG_INDEX_HPP_

#include <boost/container/map.hpp>

namespace pkg
{

class IndexEntry
{
public:
    uint32_t   offset;
    uint32_t   size;
};

typedef boost::container::map<std::string, IndexEntry> IndexMap;

}
// namespace pkg

#endif // _PKG_INDEX_HPP_
