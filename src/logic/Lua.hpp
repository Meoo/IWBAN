/**
 * @file   Lua.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LUA_HPP_
#define _IWBAN_LUA_HPP_

#include <Global.hpp>

#include <lua.hpp>

namespace logic
{

class Lua
{
private:
    lua_State * _state;

public:
    Lua();

    ~Lua();

    operator lua_State*() { return _state; }

};
// class logic

}
// namespace log

#endif // _IWBAN_LUA_HPP_
