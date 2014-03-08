/**
 * @file   Exception.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_EXCEPTION_HPP_
#define _IWBAN_EXCEPTION_HPP_

#include <Global.hpp>

#include <exception>

namespace sys
{

/**
 * @brief
 */
class Exception : public std::exception
{
public:
    virtual const char * title() const noexcept = 0;
    virtual const char * what() const noexcept = 0;

};
// class Exception

}
// namespace sys

#endif // _IWBAN_EXCEPTION_HPP_
