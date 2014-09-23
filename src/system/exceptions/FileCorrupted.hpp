/**
 * @file   FileCorrupted.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_FILECORRUPTED_HPP_
#define _IWBAN_FILECORRUPTED_HPP_

#include <Global.hpp>

#include <system/Exception.hpp>
#include <system/exceptions/DataCorrupted.hpp>

namespace sys
{

class FileCorrupted : public Exception
{
private:
    char _what[1024];


public:
    FileCorrupted(const char * file, const char * what)
    {
        std::snprintf(_what, sizeof(_what), "%s : %s", file, what);
    }

    FileCorrupted(const char * file, const DataCorrupted & exception)
    {
        std::snprintf(_what, sizeof(_what), "%s : %s", file, exception.what());
    }

    virtual const char * title() const noexcept
    {
        return "File corrupted";
    }

    virtual const char * what() const noexcept
    {
        return _what;
    }

};
// class FileCorrupted

}
// namespace sys

#endif // _IWBAN_FILECORRUPTED_HPP_
