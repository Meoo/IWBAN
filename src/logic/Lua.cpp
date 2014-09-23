/**
 * @file   Lua.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <logic/Lua.hpp>

#include <lauxlib.h>
#include <lualib.h>

namespace logic
{

Lua::Lua()
{
    _state = luaL_newstate();
    if (!_state)
        throw 1; // TODO LuaError

    luaopen_base(_state);
    luaopen_coroutine(_state);
    luaopen_table(_state);
    luaopen_string(_state);
    luaopen_bit32(_state);
    luaopen_math(_state);

    // Each luaopen_ function push a library on the stack.
    lua_pop(_state, 6);

    static const char * banned_funcs[] =
    {
        "collectgarbage",
        "dofile",
        "getmetatable",
        "loadfile",
        "load",
        "loadstring",
        "rawequal",
        "rawlen",
        "rawget",
        "rawset",
        "setmetatable",
        "getfenv",
        nullptr
    };

    // Remove banned functions
    lua_pushglobaltable(_state);
    for (unsigned i = 0; banned_funcs[i]; ++i)
    {
        lua_pushstring(_state, banned_funcs[i]);
        lua_pushnil(_state);
        lua_rawset(_state, -2);
    }
    lua_pop(_state, 1);
}

Lua::~Lua()
{
    lua_close(_state);
}

}
// namespace logic
