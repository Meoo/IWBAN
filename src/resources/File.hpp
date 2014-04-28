/**
 * @file   File.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_FILE_HPP_
#define _IWBAN_FILE_HPP_

#include <Global.hpp>

#include <boost/noncopyable.hpp>

#include <vector>

namespace res
{

class FileHandle;

namespace impl
{

class FileImpl
{
public:
    // Virtual destructor
    virtual ~FileImpl() {}

    // Virtual functions
    virtual const void * getData() const = 0;

    virtual std::size_t getSize() const = 0;

};
// class FileImpl

// ---- ---- ---- ----

class FileHandleImpl
{
public:
    // Virtual destructor
    virtual ~FileHandleImpl() {}

    // Virtual functions
    virtual FileImpl * open() = 0;

};
// class FileHandleImpl

}
// namespace impl

// ---- ---- ---- ----

/**
 * Read-only file mapped in memory.
 */
class File : public boost::noncopyable
{
public:
    friend File res::openFile(const std::string & filename);
    friend class res::FileHandle;


private:
    // Implementation
    impl::FileImpl * _impl;


public:
    // Constructors
    File();

    File(File && other);

    // Destructor
    ~File();

    // Assignment move
    File & operator = (File && other);

    // Functions
    const void * getData() const
    {
        BOOST_ASSERT(isOpen());
        return _impl->getData();
    }

    std::size_t getSize() const
    {
        BOOST_ASSERT(isOpen());
        return _impl->getSize();
    }

    bool isOpen() const
    {
        return _impl != 0;
    }


private:
    // Private constructor
    explicit File(impl::FileImpl * implementation);

};
// class File

// ---- ---- ---- ----

// TODO Should be copyable?
class FileHandle : public boost::noncopyable
{
public:
    friend FileHandle findFile(const std::string & filename);
    friend std::vector<FileHandle> findFiles(const std::string & file_pattern);


private:
    // Data members
    const std::string       _filename;

    impl::FileHandleImpl *  _localized;
    impl::FileHandleImpl *  _unlocalized;


public:
    // Constructors
    FileHandle();

    FileHandle(FileHandle && other);

    // Destructor
    ~FileHandle();

    // Assignment move
    FileHandle & operator = (FileHandle && other);

    // Functions
    // Check if the file can be opened
    bool isValid()
    {
        return _localized != 0 || _unlocalized != 0;
    }

    // Open localized, or unlocalized if it doesn't exist
    File open()
    {
        BOOST_ASSERT(isValid());
        if (isLocalized())
            return openLocalized();
        else
            return openUnlocalized();
    }

    bool isLocalized() const
    {
        return _localized != 0;
    }

    File openLocalized()
    {
        BOOST_ASSERT(isLocalized());
        return File(_localized->open());
    }

    bool isUnlocalized() const
    {
        return _unlocalized != 0;
    }

    File openUnlocalized()
    {
        BOOST_ASSERT(isUnlocalized());
        return File(_unlocalized->open());
    }

    const std::string & getFileName() const
    {
        return _filename;
    }


private:
    // Private constructor
    FileHandle(impl::FileHandleImpl * localized,
               impl::FileHandleImpl * unlocalized);

};
// class FileHandle

// ---- ---- ---- ----

// Static functions
File openFile(const std::string & filename);

FileHandle findFile(const std::string & filename);

}
// namespace res

#endif // _IWBAN_FILE_HPP_
