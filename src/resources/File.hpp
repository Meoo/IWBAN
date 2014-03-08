/**
 * @file   File.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_FILE_HPP_
#define _IWBAN_FILE_HPP_

#include <Global.hpp>

#include <boost/noncopyable.hpp>

namespace res
{

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

}
// namespace impl

// ---- ---- ---- ----

/**
 * Read-only file mapped in memory.
 */
class File : public boost::noncopyable
{
private:
    friend File res::openFile(const std::string & filename);

    // Implementation
    impl::FileImpl * _impl;


public:
    // Constructors
    File();

    File(File && other);

    // Destructor
    ~File();

    // Functions
    const void * getData() const
    {
        return _impl->getData();
    }

    std::size_t getSize() const
    {
        return _impl->getSize();
    }


private:
    // Private constructor
    explicit File(impl::FileImpl * implementation);


};
// class File

// ---- ---- ---- ----

// Static function
File openFile(const std::string & filename);

}
// namespace res

#endif // _IWBAN_FILE_HPP_
