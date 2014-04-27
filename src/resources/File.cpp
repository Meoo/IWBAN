/**
 * @file   File.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

// TODO Remove that... must find another way to fix boost's bug
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <resources/File.hpp>

#include <resources/impl/Package.hpp>
#include <resources/impl/SingleFile.hpp>

#include <system/exceptions/FileNotFound.hpp>

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

FileHandle::FileHandle(impl::FileHandleImpl* localized,
                       impl::FileHandleImpl* unlocalized)
    : _localized(localized),
      _unlocalized(unlocalized)
{
}

// ---- ---- ---- ----

File openFile(const std::string & filename)
{
    IWBAN_LOG_DEBUG("Opening file '%s'\n", filename.c_str());

    namespace fs = boost::filesystem;

#ifndef NDEBUG

    // Debug : Search on filesystem first
    std::string path = std::string(IWBAN_DATA_FOLDER "/") + filename;
    if (fs::exists(fs::path(path)))
        return File(new impl::SingleFile(path));

#endif

    // Search in packages
    std::size_t sep = filename.find_first_of('/');
    if (sep != std::string::npos)
    {
        // Get directory before first '/'
        std::string package_name = filename.substr(0, sep);
        impl::Package * package = impl::getPackage(package_name);

        if (package != 0)
        {
            // Get path after first '/'
            std::string file_id = filename.substr(sep + 1);
            impl::FileImpl * file_impl = package->openFile(file_id);

            if (file_impl)
                return File(file_impl);
        }
    }

#ifdef NDEBUG

    // Release : Search on filesystem
    // SingleFile will throw by itself if no file is found
    std::string path = std::string(IWBAN_DATA_FOLDER "/") + filename;
    return File(new impl::SingleFile(path));

#else

    // Debug : File not found
    throw sys::FileNotFound(filename.c_str());

#endif
}
// openFile()


FileHandle findFile(const std::string & filename)
{
    IWBAN_LOG_DEBUG("Finding file '%s'\n", filename.c_str());

    namespace fs = boost::filesystem;

    impl::FileHandleImpl * localized = 0;
    impl::FileHandleImpl * unlocalized = 0;

#ifndef NDEBUG

    // Debug : Search on filesystem first
    std::string path = std::string(IWBAN_DATA_FOLDER "/") + filename;
    if (fs::exists(fs::path(path)))
        unlocalized = new impl::SingleFileHandle(path);

#endif

    // Search in packages
    std::size_t sep = filename.find_first_of('/');
    if (sep != std::string::npos)
    {
        // Get directory before first '/'
        std::string package_name = filename.substr(0, sep);
        impl::Package * package = impl::getPackage(package_name);

        if (package != 0)
        {
            // Get path after first '/'
            std::string file_id = filename.substr(sep + 1);

            localized = package->findFileLocalized(file_id);

#ifndef NDEBUG
            if (unlocalized != 0)
#endif
                unlocalized = package->findFileUnlocalized(file_id);
        }
    }

#ifdef NDEBUG

    // Release : Search on filesystem
    if (unlocalized == 0)
    {
        std::string path = std::string(IWBAN_DATA_FOLDER "/") + filename;
        if (fs::exists(fs::path(path)))
            unlocalized = new impl::SingleFileHandle(path);
    }

#endif

    return FileHandle(localized, unlocalized);
}
// findFile()

}

// namespace res
