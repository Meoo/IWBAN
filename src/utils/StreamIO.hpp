/**
 * @file   StreamIO.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_STREAMIO_HPP_
#define _IWBAN_STREAMIO_HPP_

#include <Global.hpp>

#include <cstring> // strlen
#include <iostream>
#include <string>

namespace ut
{

/**
 * Read a binary value from a stream.
 *
 * All values are stored using little endian mode.
 */
template<typename T> inline
T read(std::istream & stream)
{
    IWBAN_STATIC_ASSERT(std::is_integral<T>::value);

    // TODO Endianness should not be ignored
    T value;
    stream.read(reinterpret_cast<char *>(&value), sizeof(T));
    return value;
}

/**
 * Read a null terminated string from a stream.
 */
template<> inline
std::string read<std::string>(std::istream & stream)
{
    std::string ret;
    std::getline(stream, ret, '\0');
    return ret;
}

// ---- ---- ---- ----

/**
 * Read a binary value from a stream.
 *
 * All values are stored using little endian mode.
 */
template<typename T> inline
void read(std::istream & stream, T & output)
{
    output = read<T>(stream);
}

// ---- ---- ---- ----

/**
 * Write a binary value from a stream.
 *
 * All values are stored using little endian mode.
 */
template<typename T> inline
void write(std::ostream & stream, const T & value)
{
    IWBAN_STATIC_ASSERT(std::is_integral<T>::value);

    // TODO Endianness should not be ignored
    stream.write(reinterpret_cast<const char *>(&value), sizeof(T));
}

/**
 * Write a null terminated string to a stream.
 *
 * If the string contains a null character, it is used as the end
 * of the string and the rest is discarded.
 */
template<> inline
void write<std::string>(std::ostream & stream, const std::string & value)
{
    // Do not use value.length() just in case value contains a null character
    stream.write(value.c_str(), std::strlen(value.c_str()) + 1);
}

}
// namespace ut

#endif // _IWBAN_STREAMIO_HPP_
