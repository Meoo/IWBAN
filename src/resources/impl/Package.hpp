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
    std::string     _package_name;

    // Localization
    bool            _has_loc;

    pkg::IndexMap   _loc_index;
    MappedFile      _loc_file;

    // Default package
    pkg::IndexMap   _base_index;
    MappedFile      _base_file;


public:
    // Constructor
                Package(const std::string & package_name,
                        MappedFile & loc, MappedFile & base);

    // Destructor
    // TODO Close files on destruction..?
                ~Package();

    // Functions
    FileImpl *  openFile(const std::string & filename);
    FileImpl *  openFileUnlocalized(const std::string & filename);
    FileImpl *  openFileLocalized(const std::string & filename);

    FileHandleImpl *  findFile(const std::string & filename);
    FileHandleImpl *  findFileUnlocalized(const std::string & filename);
    FileHandleImpl *  findFileLocalized(const std::string & filename);

    MappedFile &    getLocalizedSource()    { return _loc_file; }
    MappedFile &    getUnlocalizedSource()  { return _base_file; }

};
// class Package

// ---- ---- ---- ----

Package * getPackage(const std::string & package);

}
// namespace impl

}
// namespace res

#endif // _IWBAN_PACKAGE_HPP_
