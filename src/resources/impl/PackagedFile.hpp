/**
 * @file   PackagedFile.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_PACKAGEDFILE_HPP_
#define _IWBAN_PACKAGEDFILE_HPP_

#include <Global.hpp>

#include <resources/File.hpp>
#include <resources/impl/Package.hpp>

#include <packager/Index.hpp>

#include <boost/iostreams/device/mapped_file.hpp>

namespace res
{

namespace impl
{

/**
 * @brief
 */
class PackagedFile : public FileImpl
{
private:
    typedef boost::iostreams::mapped_file_source MappedFile;
    
    // Data members
    MappedFile      _source_file;

    pkg::IndexEntry _index_entry;


public:
    // Constructor
    PackagedFile(const MappedFile & source, const pkg::IndexEntry & index)
        : _source_file(source), _index_entry(index)
    {}

    // TODO Close source file on destruction?

    // Virtual functions
    virtual const void * getData() const
    {
        return _source_file.data() + _index_entry.offset;
    }

    virtual std::size_t getSize() const
    {
        return _index_entry.size;
    }

};
// class PackagedFile

}
// namespace impl

}
// namespace res

#endif // _IWBAN_PACKAGEDFILE_HPP_
