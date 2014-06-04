/**
 * @file   StreamIO.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_STREAMIO_HPP_
#define _IWBAN_STREAMIO_HPP_

#include <Global.hpp>

#include <iostream>
#include <string>

namespace ut
{

template<typename T> inline
T read(std::istream & stream)
{
    T value;
    stream.read(reinterpret_cast<char *>(&value), sizeof(T));
    return value;
}

template<> inline
std::string && read(std::istream & stream)
{
    std::string ret;
    std::getline(stream, ret, '\0');
    return std::move(ret);
}

// ---- ---- ---- ----

template<typename T> inline
void write(std::ostream & stream, const T & value)
{
    stream.write(reinterpret_cast<const char *>(&value), sizeof(T));
}

template<> inline
void write(std::ostream & stream, const std::string & value)
{
    // Do not use value.length() just in case value contains a null character
    stream.write(value.c_str(), std::strlen(value.c_str()) + 1);
}

}
// namespace ut

#endif // _IWBAN_STREAMIO_HPP_
