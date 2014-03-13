/**
 * @file   Package.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_PACKAGE_HPP_
#define _IWBAN_PACKAGE_HPP_

#include <Global.hpp>

#include <resources/File.hpp>

#include <packager/Index.hpp>

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
public:
    typedef boost::iostreams::mapped_file_source MappedFile;


private:
    // Data members

    // Localization
    bool            _has_loc;

    pkg::IndexMap   _loc_index;
    MappedFile      _loc_file;

    // Default package
    pkg::IndexMap   _base_index;
    MappedFile      _base_file;


public:
    // Constructor
                Package(MappedFile & loc, MappedFile & base);

    // Destructor
    // TODO Close files on destruction..?
                ~Package();

    // Accessor
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
