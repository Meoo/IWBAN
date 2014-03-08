/**
 * @file   FileCorrupted.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_FILECORRUPTED_HPP_
#define _IWBAN_FILECORRUPTED_HPP_

#include <Global.hpp>

#include <system/Exception.hpp>

namespace sys
{

class FileCorrupted : public Exception
{
private:
    char _what[1024];


public:
    FileCorrupted(const char * filename)
    {
        std::snprintf(_what, sizeof(_what),
                      "File \"%s\" is unreadable", filename);
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
