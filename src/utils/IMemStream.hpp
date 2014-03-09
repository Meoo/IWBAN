/**
 * @file   IMemStream.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_IMEMSTREAM_HPP_
#define _IWBAN_IMEMSTREAM_HPP_

#include <streambuf>
#include <istream>

/*
 * Original code from http://stackoverflow.com/a/13059195
 */

namespace ut
{

class MemBuffer : std::streambuf
{
public:
    MemBuffer(char const * base, size_t size)
    {
        char* p(const_cast<char*>(base));
        this->setg(p, p, p + size);
    }
};

// ---- ---- ---- ----

class IMemStream: virtual MemBuffer, std::istream
{
public:
    IMemStream(char const * base, size_t size)
        : MemBuffer(base, size)
        , std::istream(static_cast<std::streambuf*>(this))
    {}
};

}
// namespace ut

#endif // _IWBAN_IMEMSTREAM_HPP_
