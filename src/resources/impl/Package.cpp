/**
 * @file   Package.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

// TODO Remove that... must find another way to fix boost's bug
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <config/PackageConfig.hpp>
#include <config/Settings.hpp>

#include <resources/FileStream.hpp>
#include <resources/impl/Package.hpp>
#include <resources/impl/PackagedFile.hpp>

#include <system/exceptions/FileCorrupted.hpp>
#include <system/exceptions/FileNotFound.hpp>

#include <map>
#include <memory>

namespace
{

typedef std::unique_ptr<res::impl::Package> PackagePtr;

class PackageEntry
{
public:
    PackagePtr localized;
    PackagePtr unlocalized;
};

typedef std::map<std::string, PackageEntry> PackageMap;

// ---- ---- ---- ----

static PackageMap s_package_map;

// ---- ---- ---- ----

PackageMap::iterator loadPackage(const std::string & package)
{
    PackageEntry entry;

    // Unlocalized
    try
    {
        std::string unloc_path = std::string(IWBAN_DATA_FOLDER "/")
                               + package + IWBAN_PKG_EXTENSION;

        entry.unlocalized.reset(new res::impl::Package(unloc_path));

        IWBAN_LOG_INFO("Package '%s' found\n", package.c_str());
    }
    catch (sys::FileNotFound & ignored)
    {
        IWBAN_LOG_INFO("Package '%s' not found\n", package.c_str());
    }

    // Localized
    try
    {
        std::string loc_path = std::string(IWBAN_DATA_FOLDER "/")
                             + package + "." + cfg::language + IWBAN_PKG_EXTENSION;

        entry.localized.reset(new res::impl::Package(loc_path));

        IWBAN_LOG_INFO("Package '%s.%s' found\n",
            package.c_str(), cfg::language.c_str());
    }
    catch (sys::FileNotFound & ignored)
    {
        IWBAN_LOG_INFO("Package '%s.%s' not found\n",
            package.c_str(), cfg::language.c_str());
    }

    // Insert the package into the map
    auto ret = s_package_map.insert(std::make_pair(package, std::move(entry)));

    // TODO Better than that?
    IWBAN_ASSERT(ret.second);

    return ret.first;

}
// loadPackage()

}
// namespace

// ---- ---- ---- ----

namespace res
{

namespace impl
{

Package::Package(const std::string & package_filename)
{
    // Wrap boost exception using our own exceptions
    try
    {
        _source.open(package_filename);
    }
    catch (/* TODO boost::file_not_found */...)
    {
        throw sys::FileNotFound(package_filename.c_str());
    }

    // Create stream
    FileStream stream(_source.data(), _source.size());

    // Read index
    if (!readIndex(stream, _index))
        throw sys::FileCorrupted(package_filename.c_str());
}

FileImpl * Package::openFile(const std::string & filename)
{
    Index::iterator it = _index.find(filename);

    if (it != _index.end())
        return new PackagedFile(_source, it->second);

    return nullptr;
}

// ---- ---- ---- ----

FileImpl * openFileInPackage(const std::string & package,
                             const std::string & filename)
{
    PackageMap::iterator it = s_package_map.find(package);

    if (it == s_package_map.end())
        it = loadPackage(package);

    // Localized
    if (it->second.localized)
        return it->second.localized->openFile(filename);

    // Unlocalized
    if (it->second.unlocalized)
        return it->second.unlocalized->openFile(filename);

    return nullptr;
}

}
// namespace impl

}
// namespace res
