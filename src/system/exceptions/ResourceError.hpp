/**
 * @file   ResourceError.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_RESOURCEERROR_HPP_
#define _IWBAN_RESOURCEERROR_HPP_

#include <Global.hpp>

#include <system/Exception.hpp>

namespace sys
{

class ResourceError : public Exception
{
private:
    char _what[1024];


public:
    ResourceError(const char * what)
    {
        std::snprintf(_what, sizeof(_what), "%s", what);
    }

    virtual const char * title() const noexcept
    {
        return "Resource error";
    }

    virtual const char * what() const noexcept
    {
        return _what;
    }

};
// class ResourceError

}
// namespace sys

#endif // _IWBAN_RESOURCEERROR_HPP_
