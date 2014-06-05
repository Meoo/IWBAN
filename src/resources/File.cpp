/**
 * @file   File.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

// TODO Remove that... must find another way to fix boost's bug
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <config/Settings.hpp>

#include <resources/File.hpp>
#include <resources/impl/Package.hpp>
#include <resources/impl/SingleFile.hpp>

#include <system/exceptions/FileNotFound.hpp>
#include <system/exceptions/ResourceError.hpp>

#include <boost/filesystem.hpp>

namespace res
{

File::File(File && other)
    : _impl(std::move(other._impl))
{
}

File & File::operator = (File && other)
{
    _impl = std::move(other._impl);
    return *this;
}

File::File(impl::FileImpl * implementation)
    : _impl(implementation)
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
    {
        impl::FileImpl * file_impl = impl::openFileInPackage(package_name, file_id);

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

}
// namespace res
