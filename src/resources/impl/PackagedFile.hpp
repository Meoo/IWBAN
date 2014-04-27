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

// ---- ---- ---- ----

class PackagedFileHandle : public FileHandleImpl
{
private:
    // Data members
    Package *       _package;
    pkg::IndexEntry _index_entry;
    bool            _localized;


public:
    // Constructor
    explicit PackagedFileHandle(Package * package,
                                const pkg::IndexEntry & index_entry,
                                bool localized)
        : _package(package), _index_entry(index_entry), _localized(localized)
    {
    }

    // Virtual functions
    virtual FileImpl * open()
    {
        if (_localized)
            return new PackagedFile(_package->getLocalizedSource(), _index_entry);
        else
            return new PackagedFile(_package->getUnlocalizedSource(), _index_entry);
    }

};
// class SingleFileHandle

}
// namespace impl

}
// namespace res

#endif // _IWBAN_PACKAGEDFILE_HPP_
