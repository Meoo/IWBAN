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

#include <packager/Index.hpp>

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
    typedef boost::iostreams::mapped_file_source MappedFile;

    // Data members
    pkg::IndexMap   _index;

    MappedFile      _file;


public:
                Package(const std::string & package_name);

    FileImpl *  findFile(const std::string & filename);

};
// class Package

// ---- ---- ---- ----

Package * getPackage(const std::string & package);

}
// namespace impl

}
// namespace res

#endif // _IWBAN_PACKAGE_HPP_
