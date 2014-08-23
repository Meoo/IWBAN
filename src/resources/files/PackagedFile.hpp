/**
 * @file   PackagedFile.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_PACKAGEDFILE_HPP_
#define _IWBAN_PACKAGEDFILE_HPP_

#include <Global.hpp>

#include <resources/File.hpp>
#include <resources/Index.hpp>

#include <boost/iostreams/device/mapped_file.hpp>

namespace res
{

class PackagedFile : public FileImpl
{
private:
    typedef boost::iostreams::mapped_file_source Source;

    // Data members
    Source      _source;

    IndexEntry  _index_entry;


public:
    // Constructor
    PackagedFile(const Source & source, const IndexEntry & index)
        : _source(source), _index_entry(index)
    {}

    // Virtual functions
    const void * getData() const override
    {
        return _source.data() + _index_entry.offset;
    }

    std::size_t getSize() const override
    {
        return _index_entry.size;
    }

};
// class PackagedFile

}
// namespace res

#endif // _IWBAN_PACKAGEDFILE_HPP_
