/**
 * @file   DataCorrupted.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_DATACORRUPTED_HPP_
#define _IWBAN_DATACORRUPTED_HPP_

#include <Global.hpp>

#include <system/Exception.hpp>

namespace sys
{

class DataCorrupted : public Exception
{
private:
    char _what[1024];


public:
    DataCorrupted(const char * what)
    {
        std::snprintf(_what, sizeof(_what), "%s", what);
    }

    virtual const char * title() const noexcept
    {
        return "Data corrupted";
    }

    virtual const char * what() const noexcept
    {
        return _what;
    }

};
// class DataCorrupted

}
// namespace sys

#endif // _IWBAN_DATACORRUPTED_HPP_
