/**
 * @file   FileNotFound.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_FILENOTFOUND_HPP_
#define _IWBAN_FILENOTFOUND_HPP_

#include <Global.hpp>

#include <system/Exception.hpp>

namespace sys
{

class FileNotFound : public Exception
{
private:
    char _what[1024];


public:
    FileNotFound(const char * filename)
    {
        std::snprintf(_what, sizeof(_what),
                      "Unable to locate file \"%s\"", filename);
    }

    virtual const char * title() const noexcept
    {
        return "File not found";
    }

    virtual const char * what() const noexcept
    {
        return _what;
    }

};
// class FileNotFound

}
// namespace sys

#endif // _IWBAN_FILENOTFOUND_HPP_
