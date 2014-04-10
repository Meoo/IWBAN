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

// ---- ---- ---- ----

File::File(impl::FileImpl * implementation)
    : _impl(implementation)
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
            impl::FileImpl * file_impl = package->findFile(file_id);

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

}
// namespace res
