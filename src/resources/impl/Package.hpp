/**
 * @file   Package.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_PACKAGE_HPP_
#define _IWBAN_PACKAGE_HPP_

#include <Global.hpp>

#include <resources/File.hpp>

#include <boost/container/map.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

namespace res
{

namespace impl
{

/**
 * @brief
 */
class Package
{
private:
    class IndexEntry
    {
    public:
        uint32_t   offset;
        uint32_t   size;
    };

    typedef boost::container::map<std::string, IndexEntry> IndexMap;

    typedef boost::iostreams::mapped_file_source MappedFile;

    // Data members
    IndexMap    _index;

    MappedFile  _file;


public:
                Package(const std::string & package_name);

    FileImpl *  findFile(const std::string & filename) { return 0; }


private:
    void        readIndex();

};
// class Package

// ---- ---- ---- ----

Package * getPackage(const std::string & package) { return 0; }

}
// namespace impl

}
// namespace res

#endif // _IWBAN_PACKAGE_HPP_
