/**
 * @file   FileStream.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_FILESTREAM_HPP_
#define _IWBAN_FILESTREAM_HPP_

#include <Global.hpp>

#include <resources/File.hpp>

#include <sstream>

namespace res
{

// Do not close the file while the stream is in use
class FileStream : public std::istringstream
{
public:
    explicit FileStream(File & file)
    {
        rdbuf()->pubsetbuf((char*) const_cast<void*>(file.getData()), file.getSize());
    }

    FileStream(const void * data, std::size_t size)
    {
        rdbuf()->pubsetbuf((char*) const_cast<void*>(data), size);
    }

};
// class FileStream

}
// namespace res

#endif // _IWBAN_FILESTREAM_HPP_
