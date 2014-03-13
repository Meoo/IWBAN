/**
 * @file   Package.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

// TODO Remove that... must find another way to fix boost's bug
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <config/Config.hpp>

#include <resources/impl/Package.hpp>
#include <resources/impl/PackagedFile.hpp>

#include <system/exceptions/FileCorrupted.hpp>

#include <packager/PackagerConfig.hpp>
#include <packager/ReadIndex.hpp>

#include <sstream>

namespace res
{

namespace impl
{

typedef boost::container::map<std::string, Package *> PackageMap;

// ---- ---- ---- ----

Package::Package(MappedFile & loc, MappedFile & base)
    : _loc_file(loc), _base_file(base)
{
    // TODO Handle errors with real filenames
    _has_loc = loc.is_open();
    if (_has_loc)
    {
        // Create stream
        std::istringstream loc_st;
        loc_st.rdbuf()->pubsetbuf(
                const_cast<char*>(_loc_file.data()), _loc_file.size());
        
        // Read index
        if (!pkg::readIndex(loc_st, _loc_index))
            throw sys::FileCorrupted("Loc");
    }

    // Create stream
    std::istringstream base_st;
    base_st.rdbuf()->pubsetbuf(
            const_cast<char*>(_base_file.data()), _base_file.size());
    
    // Read index
    if (!pkg::readIndex(base_st, _base_index))
        throw sys::FileCorrupted("Base");
}

FileImpl * Package::findFile(const std::string & filename)
{
    // Localization
    if (_has_loc)
    {
        pkg::IndexMap::iterator it = _loc_index.find(filename);
        if (it != _loc_index.end())
            return new PackagedFile(_loc_file, it->second);
    }

    // Base
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
        return it->second;
    
    else
    {
        // Base
        std::string base_path = std::string(IWBAN_DATA_FOLDER "/")
                + package + PKG_EXTENSION;
        Package::MappedFile base;

        try { base.open(base_path); }
        catch (...) {}
        
        if (!base.is_open())
        {
            // No package found
            IWBAN_LOG_INFO("Package '%s' not found\n", package.c_str());
            s_packageMap[package] = 0;
            return 0;
        }
        
        // Localization
        std::string loc_path = std::string(IWBAN_DATA_FOLDER "/")
                + package + "." + cfg::language + PKG_EXTENSION;
        Package::MappedFile loc;

        try { loc.open(loc_path); }
        catch (...) {}

        // Finally, create package
        Package * p = new Package(loc, base);
        s_packageMap[package] = p;
        
        IWBAN_LOG_INFO("Package '%s' found (%s '%s' localization)\n",
                package.c_str(), loc.is_open() ? "with" : "without",
                cfg::language.c_str());
        
        return p;
    }
}

}
// namespace impl

}
// namespace res
