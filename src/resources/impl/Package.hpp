/**
 * @file   Package.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_PACKAGE_HPP_
#define _IWBAN_PACKAGE_HPP_

#include <Global.hpp>

#include <resources/File.hpp>
#include <resources/impl/Index.hpp>

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
    typedef boost::iostreams::mapped_file_source Source;


private:
    // Data members
    std::string     _package_name;

    Index           _index;
    Source          _source;


public:
    // Constructor
                Package(const std::string & package_filename);

    // Functions
    FileImpl *  openFile(const std::string & filename);

};
// class Package

// ---- ---- ---- ----

FileImpl * openFileInPackage(const std::string & package,
                             const std::string & filename);

}
// namespace impl

}
// namespace res

#endif // _IWBAN_PACKAGE_HPP_
