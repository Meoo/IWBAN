/**
 * @file   Package.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

// TODO Remove that... must find another way to fix boost's bug
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <config/Config.hpp>

#include <resources/FileStream.hpp>
#include <resources/impl/Package.hpp>
#include <resources/impl/PackagedFile.hpp>

#include <system/exceptions/FileCorrupted.hpp>

#include <packager/PackagerConfig.hpp>
#include <packager/ReadIndex.hpp>

#include <map>

namespace res
{

namespace impl
{

typedef std::map<std::string, Package *> PackageMap;

// ---- ---- ---- ----

Package::Package(const std::string & package_name,
                 MappedFile & loc, MappedFile & base)
    : _package_name(package_name), _loc_file(loc), _base_file(base)
{
    // TODO MappedFiles should be loaded from inside this function, and not in getPackage
    _has_loc = loc.is_open();
    if (_has_loc)
    {
        // Create stream
        FileStream loc_st(_loc_file.data(), _loc_file.size());

        // Read index
        if (!pkg::readIndex(loc_st, _loc_index))
            throw sys::FileCorrupted((_package_name + "."
                    + cfg::language + PKG_EXTENSION).c_str());
    }

    // Create stream
    std::istringstream base_st;
    base_st.rdbuf()->pubsetbuf(
            const_cast<char*>(_base_file.data()), _base_file.size());

    // Read index
    if (!pkg::readIndex(base_st, _base_index))
        throw sys::FileCorrupted((_package_name + PKG_EXTENSION).c_str());
}

FileImpl * Package::openFile(const std::string & filename)
{
    FileImpl * file = openFileLocalized(filename);
    if (file)
        return file;

    return openFileUnlocalized(filename);
}

FileImpl * Package::openFileUnlocalized(const std::string & filename)
{
    // Unlocalized
    pkg::IndexMap::iterator it = _base_index.find(filename);
    if (it != _base_index.end())
        return new PackagedFile(_base_file, it->second);

    return 0;
}

FileImpl * Package::openFileLocalized(const std::string & filename)
{
    // Localized
    if (_has_loc)
    {
        pkg::IndexMap::iterator it = _loc_index.find(filename);
        if (it != _loc_index.end())
            return new PackagedFile(_loc_file, it->second);
    }

    return 0;
}

FileHandleImpl * Package::findFile(const std::string& filename)
{
    FileHandleImpl * file = findFileLocalized(filename);
    if (file)
        return file;

    return findFileUnlocalized(filename);
}

FileHandleImpl * Package::findFileUnlocalized(const std::string& filename)
{
    // Unlocalized
    pkg::IndexMap::iterator it = _base_index.find(filename);
    if (it != _base_index.end())
        return new PackagedFileHandle(this, it->second, true);

    return 0;
}

FileHandleImpl * Package::findFileLocalized(const std::string& filename)
{
    // Localized
    pkg::IndexMap::iterator it = _loc_index.find(filename);
    if (it != _loc_index.end())
        return new PackagedFileHandle(this, it->second, false);

    return 0;
}

// ---- ---- ---- ----

Package * getPackage(const std::string & package)
{
    static PackageMap s_package_map;

    PackageMap::iterator it = s_package_map.find(package);

    if (it != s_package_map.end())
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
            s_package_map[package] = 0;
            return 0;
        }

        // Localization
        std::string loc_path = std::string(IWBAN_DATA_FOLDER "/")
                + package + "." + cfg::language + PKG_EXTENSION;
        Package::MappedFile loc;

        try { loc.open(loc_path); }
        catch (...) {}

        // Finally, create package
        Package * p = new Package(package, loc, base);
        s_package_map[package] = p;

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
