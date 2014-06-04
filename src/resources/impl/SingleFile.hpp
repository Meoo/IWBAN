/**
 * @file   SingleFile.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SINGLEFILE_HPP_
#define _IWBAN_SINGLEFILE_HPP_

#include <Global.hpp>

#include <resources/File.hpp>

#include <system/exceptions/FileNotFound.hpp>

#include <boost/iostreams/device/mapped_file.hpp>

namespace res
{

namespace impl
{

class SingleFile : public FileImpl
{
private:
    typedef boost::iostreams::mapped_file_source MappedFile;

    // Data members
    MappedFile _file;


public:
    // Constructor
    explicit SingleFile(const std::string & filename)
        : _file(filename.c_str())
    {
        if (!_file.is_open())
            throw sys::FileNotFound(filename.c_str());
    }

    // Destructor
    virtual ~SingleFile()
    {
        if (_file.is_open())
            _file.close();
    }

    // Virtual functions
    virtual const void * getData() const
    {
        return _file.data();
    }

    virtual std::size_t getSize() const
    {
        return _file.size();
    }

};
// class SingleFile

}
// namespace impl

}
// namespace res

#endif // _IWBAN_SINGLEFILE_HPP_
