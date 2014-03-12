/**
 * @file   Package.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

// TODO Remove that... must find another way to fix boost's bug
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <resources/impl/Package.hpp>
#include <resources/impl/PackagedFile.hpp>

#include <packager/PackagerConfig.hpp>
#include <packager/ReadIndex.hpp>

namespace res
{

namespace impl
{

typedef boost::container::map<std::string, Package *> PackageMap;

// ---- ---- ---- ----

Package::Package(const std::string & package_name)
{
}

FileImpl * Package::findFile(const std::string & filename)
{
    if (_has_loc)
    {
        pkg::IndexMap::iterator it = _loc_index.find(filename);
        if (it != _loc_index.end())
            return new PackagedFile(_loc_file, it->second);
    }

    pkg::IndexMap::iterator it = _base_index.find(filename);
    if (it != _base_index.end())
        return new PackagedFile(_base_file, it->second);
    
    return 0;
}

// ---- ---- ---- ----

Package * getPackage(const std::string & package)
{
    static PackageMap s_packageMap;

    PackageMap::iterator it = s_packageMap.find(package);
    
    if (it != s_packageMap.end())
        // Hit
        return it->second;
    
    else
    {
        // TODO Miss
        // Create Package and put it into the package map
        

        return 0;
    }
}

}
// namespace impl

}
// namespace res
