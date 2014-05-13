/**
 * @file   RestartApp.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_RESTARTAPP_HPP_
#define _IWBAN_RESTARTAPP_HPP_

#include <Global.hpp>

namespace sys
{

class RestartApp : public std::exception
{
private:
    const char * _what;


public:
    RestartApp(const char * what)
        : _what(what)
    {}

    virtual const char * what() const noexcept
    {
        return _what;
    }

};
// class RestartApp

}
// namespace sys

#endif // _IWBAN_RESTARTAPP_HPP_
