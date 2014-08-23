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

class SingleFile : public FileImpl
{
private:
    typedef boost::iostreams::mapped_file_source Source;

    // Data members
    Source _source;


public:
    // Constructor
    explicit SingleFile(const std::string & filename)
        : _source(filename.c_str())
    {
        if (!_source.is_open())
            throw sys::FileNotFound(filename.c_str());
    }

    // Destructor
    virtual ~SingleFile()
    {
        if (_source.is_open())
            _source.close();
    }

    // Virtual functions
    const void * getData() const override
    {
        return _source.data();
    }

    std::size_t getSize() const override
    {
        return _source.size();
    }

};
// class SingleFile

}
// namespace res

#endif // _IWBAN_SINGLEFILE_HPP_
