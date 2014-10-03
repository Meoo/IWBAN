/**
 * @file   Lua.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <logic/Lua.hpp>
#include <resources/File.hpp>

#include <lauxlib.h>
#include <lualib.h>

IWBAN_STATIC_ASSERT(LUA_VERSION_NUM == 502);

namespace logic
{

LuaScript LuaScript::fromFile(const std::string & filename)
{
    res::File file = res::openFile(filename);
    return LuaScript(filename, (const char *) file.getData(), file.getSize());
}

// ---- ---- ---- ----

Lua::Lua()
{
    _state = luaL_newstate();
    if (!_state)
        throw 1; // FIXME LuaError

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
        nullptr
    };

    // Remove banned functions
    lua_pushglobaltable(_state);
    for (unsigned i = 0; banned_funcs[i]; ++i)
    {
        lua_pushnil(_state);
        lua_setfield(_state, -2, banned_funcs[i]);
    }
    lua_pop(_state, 1);
}

Lua::~Lua()
{
    lua_close(_state);
}

void Lua::run(const LuaScript & script, const char * source)
{
    luaL_loadbuffer(_state, script.getData(), script.getSize(), source);
    pcall(0, 0);
}

void Lua::runFile(const std::string & filename)
{
    {
        res::File f = res::openFile(filename);
        luaL_loadbuffer(_state, (const char*) f.getData(), f.getSize(), filename.c_str());
    }
    pcall(0, 0);
}

bool Lua::pcall(int narg, int nres)
{
    int ret = lua_pcall(_state, narg, nres, 0);

    if (ret)
    {
        const char * msg = lua_tostring(_state, -1);
        const char * what;
        switch (ret)
        {
        case LUA_ERRRUN: what = "Runtime";          break;
        case LUA_ERRMEM: what = "Memory";           break;
        case LUA_ERRERR: what = "Error handler";    break;
        default:         what = "Unknown";          break;
        }

        IWBAN_LOG_ERROR("Lua (%s): %s", what, msg);

        lua_pop(_state, 1);
    }

    return ret == 0;
}

}
// namespace logic
