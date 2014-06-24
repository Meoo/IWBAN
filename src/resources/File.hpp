/**
 * @file   File.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_FILE_HPP_
#define _IWBAN_FILE_HPP_

#include <Global.hpp>

#include <boost/noncopyable.hpp>

#include <vector>
#include <memory>

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
public:
    friend File res::openFile(const std::string & filename);


private:
    // Implementation
    std::unique_ptr<impl::FileImpl> _impl;


public:
    // Constructors
    File() = default;

    File(File && other);

    // Assignment move
    File & operator = (File && other);

    // Functions
    const void * getData() const
    {
        IWBAN_PRE(isOpen());
        return _impl->getData();
    }

    std::size_t getSize() const
    {
        IWBAN_PRE(isOpen());
        return _impl->getSize();
    }

    bool isOpen() const
    {
        return static_cast<bool>(_impl);
    }


private:
    // Private constructor
    explicit File(impl::FileImpl * implementation);

};
// class File

// ---- ---- ---- ----

// Static functions
File openFile(const std::string & filename);

}
// namespace res

#endif // _IWBAN_FILE_HPP_
