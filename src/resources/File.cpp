/**
 * @file   File.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

// TODO Remove that... must find another way to fix boost's bug
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <config/Config.hpp>

#include <resources/File.hpp>
#include <resources/impl/Package.hpp>
#include <resources/impl/SingleFile.hpp>

#include <system/exceptions/FileNotFound.hpp>
#include <system/exceptions/ResourceError.hpp>

#include <boost/filesystem.hpp>

namespace res
{

File::File()
    : _impl(0)
{
}

File::File(File && other)
    : _impl(other._impl)
{
    other._impl = 0;
}

File::~File()
{
    delete _impl;
}

File & File::operator = (File && other)
{
    delete _impl;
    _impl = other._impl;
    other._impl = 0;
    return *this;
}

File::File(impl::FileImpl * implementation)
    : _impl(implementation)
{
}

// ---- ---- ---- ----

FileHandle::FileHandle()
{
    _localized = 0;
    _unlocalized = 0;
}

FileHandle::FileHandle(FileHandle && other)
    : _localized(other._localized),
      _unlocalized(other._unlocalized)
{
    other._localized = 0;
    other._unlocalized = 0;
}

FileHandle::~FileHandle()
{
    delete _localized;
    delete _unlocalized;
}

FileHandle & FileHandle::operator = (FileHandle && other)
{
    delete _localized;
    delete _unlocalized;
    _localized = other._localized;
    _unlocalized = other._unlocalized;
    other._localized = 0;
    other._unlocalized = 0;
    return *this;
}

FileHandle::FileHandle(impl::FileHandleImpl* localized,
                       impl::FileHandleImpl* unlocalized)
    : _localized(localized),
      _unlocalized(unlocalized)
{
}

// ---- ---- ---- ----

File openFile(const std::string & filename)
{
    namespace fs = boost::filesystem;

    IWBAN_LOG_DEBUG("Opening file '%s'\n", filename.c_str());

    // Search in packages
    std::size_t sep = filename.find_first_of('/');

    if (sep == std::string::npos)
        throw sys::ResourceError("All files must be contained within a package");

    // Get directory before first '/'
    std::string package_name = filename.substr(0, sep);
    // Get path after first '/'
    std::string file_id = filename.substr(sep + 1);

#ifndef NDEBUG
    // Debug : Search on filesystem first
    {
        // Localized
        std::string file_path = std::string(IWBAN_DATA_FOLDER "/")
            + package_name + "." + cfg::language + "/" + file_id;
        if (fs::exists(fs::path(file_path)))
            return File(new impl::SingleFile(file_path));
    }
    {
        // Unlocalized
        std::string file_path = std::string(IWBAN_DATA_FOLDER "/") + filename;
        if (fs::exists(fs::path(file_path)))
            return File(new impl::SingleFile(file_path));
    }
#endif

    // Search in packages
    impl::Package * package = impl::getPackage(package_name);
    if (package != 0)
    {
        impl::FileImpl * file_impl = package->openFile(file_id);

        if (file_impl)
            return File(file_impl);
    }

#ifdef NDEBUG
    // Release : Search on filesystem last
    {
        // Localized
        std::string file_path = std::string(IWBAN_DATA_FOLDER "/")
            + package_name + "." + cfg::language + "/" + file_id;
        if (fs::exists(fs::path(file_path)))
            return File(new impl::SingleFile(file_path));
    }
    {
        // Unlocalized
        std::string file_path = std::string(IWBAN_DATA_FOLDER "/") + filename;
        if (fs::exists(fs::path(file_path)))
            return File(new impl::SingleFile(file_path));
    }
#endif

    // Debug : File not found
    throw sys::FileNotFound(filename.c_str());
}
// openFile()


FileHandle findFile(const std::string & filename)
{
    namespace fs = boost::filesystem;

    IWBAN_LOG_DEBUG("Finding file '%s'\n", filename.c_str());

    impl::FileHandleImpl * localized = 0;
    impl::FileHandleImpl * unlocalized = 0;

    // Search in packages
    std::size_t sep = filename.find_first_of('/');

    if (sep == std::string::npos)
        throw sys::ResourceError("All files must be contained within a package");

    // Get directory before first '/'
    std::string package_name = filename.substr(0, sep);
    // Get path after first '/'
    std::string file_id = filename.substr(sep + 1);

#ifndef NDEBUG
    // Debug : Search on filesystem first
    {
        // Localized
        std::string file_path = std::string(IWBAN_DATA_FOLDER "/")
            + package_name + "." + cfg::language + "/" + file_id;
        if (fs::exists(fs::path(file_path)))
            localized = new impl::SingleFileHandle(file_path);
    }
    {
        // Unlocalized
        std::string file_path = std::string(IWBAN_DATA_FOLDER "/") + filename;
        if (fs::exists(fs::path(file_path)))
            unlocalized = new impl::SingleFileHandle(file_path);
    }
#endif

    impl::Package * package = impl::getPackage(package_name);
    if (package != 0)
    {
        // Theses if statements should be optimized out in Release mode
        if (localized == 0)
            localized = package->findFileLocalized(file_id);

        if (unlocalized == 0)
            unlocalized = package->findFileUnlocalized(file_id);
    }

#ifdef NDEBUG
    // Release : Search on filesystem last
    if (localized == 0)
    {
        // Localized
        std::string file_path = std::string(IWBAN_DATA_FOLDER "/")
            + package_name + "." + cfg::language + "/" + file_id;
        if (fs::exists(fs::path(file_path)))
            localized = new impl::SingleFileHandle(file_path);
    }
    if (unlocalized == 0)
    {
        // Unlocalized
        std::string file_path = std::string(IWBAN_DATA_FOLDER "/") + filename;
        if (fs::exists(fs::path(file_path)))
            unlocalized = new impl::SingleFileHandle(file_path);
    }
#endif

    return FileHandle(localized, unlocalized);
}
// findFile()

}

// namespace res
