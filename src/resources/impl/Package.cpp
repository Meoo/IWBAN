/**
 * @file   Package.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

// TODO Remove that... must find another way to fix boost's bug
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <resources/impl/Package.hpp>

#include <packager/PackagerConfig.hpp>
#include <packager/ReadIndex.hpp>

namespace res
{

namespace impl
{

Package::Package(const std::string & package_name)
{
}

FileImpl * Package::findFile(const std::string & filename)
{
    return 0;
}

// ---- ---- ---- ----

Package * getPackage(const std::string & package)
{
    return 0;
}

}
// namespace impl

}
// namespace res
