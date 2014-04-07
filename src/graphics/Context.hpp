/**
 * @file   Context.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CONTEXT_HPP_
#define _IWBAN_CONTEXT_HPP_

#include <Global.hpp>

#include <boost/noncopyable.hpp>

namespace gfx
{

class Context : public boost::noncopyable
{
public:
    // Virtual destructor
    virtual ~Context() {}

    // Virtual functions
    virtual bool isOpen() const = 0;

    virtual void close() = 0;

};
// class Context

}
// namespace gfx

#endif // _IWBAN_CONTEXT_HPP_
