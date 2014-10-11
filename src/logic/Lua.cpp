/**
 * @file   Lua.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <logic/Lua.hpp>
#include <logic/Variant.hpp>
#include <resources/File.hpp>

#include <lauxlib.h>
#include <lualib.h>

namespace
{

IWBAN_STATIC_ASSERT(LUA_VERSION_NUM == 502);

const char * IWBAN_LUA_BANNED_FUNCTIONS[] =
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

}
// namespace

namespace logic
{

LuaScript LuaScript::fromFile(const std::string & filename)
{
    res::File file = res::openFile(filename);
    return LuaScript(filename, (const char *) file.getData(), file.getSize());
}

// ---- ---- ---- ----

LuaObject::LuaObject(LuaObject && object)
    : _lua(object._lua), _registry(object._registry), _index(object._index)
{
    object._lua = nullptr;
}

LuaObject::LuaObject(Lua * lua, LuaRegistry registry)
    : _lua(lua), _registry(registry)
{
    lua_rawgeti(*_lua, LUA_REGISTRYINDEX, _registry);
    _index = lua_rawlen(*_lua, -1) + 1;

    lua_newtable(*_lua);
    lua_rawseti(*_lua, -2, _index);

    lua_pop(*_lua, 1);
}

LuaObject::~LuaObject()
{
    if (isValid())
    {
        // TODO Exception safety?
        lua_rawgeti(*_lua, LUA_REGISTRYINDEX, _registry);

        lua_pushnil(*_lua);
        lua_rawseti(*_lua, -2, _index);

        lua_pop(*_lua, 1);
    }
}

LuaObject & LuaObject::operator= (LuaObject && object)
{
    _lua        = object._lua;
    _registry   = object._registry;
    _index      = object._index;
    object._lua = nullptr;
    return *this;
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

    // Remove banned functions
    lua_pushglobaltable(_state);
    for (unsigned i = 0; IWBAN_LUA_BANNED_FUNCTIONS[i]; ++i)
    {
        lua_pushnil(_state);
        lua_setfield(_state, -2, IWBAN_LUA_BANNED_FUNCTIONS[i]);
    }
    lua_pop(_state, 1);

    // Create registry tables
    for (int i = LUA_IWBAN__FIRST; i <= LUA_IWBAN__LAST; ++i)
    {
        lua_newtable(_state);
        lua_rawseti(_state, LUA_REGISTRYINDEX, i);
    }
}

Lua::~Lua()
{
    lua_close(_state);
}

void Lua::run(const LuaScript & script)
{
    IWBAN_PRE(script.isValid());

    luaL_loadbuffer(_state, script.getData(), script.getSize(), script.getSource());
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

LuaObject Lua::createObject(LuaRegistry registry)
{
    return LuaObject(this, registry);
}

void Lua::pushObject(LuaObject & object)
{
    if (object.isValid())
    {
        lua_rawgeti(_state, LUA_REGISTRYINDEX, object.getRegistry());
        lua_rawgeti(_state, -1, object.getIndex());
        lua_remove(_state, -2);
    }
    else
        lua_pushnil(_state);
}

Variant Lua::toVariant(int index)
{
    switch (lua_type(_state, index))
    {
    case LUA_TNUMBER:
        return Variant((Variant::Float) lua_tonumber(_state, index));

    case LUA_TBOOLEAN:
        return Variant(lua_toboolean(_state, index));

    case LUA_TSTRING:
        return Variant(lua_tostring(_state, index));

    case LUA_TTABLE:
    {
        // Vector (x and y in a table)
        ut::Vector v;

        lua_getfield(_state, index, "x");
        if (!lua_isnumber(_state, -1))
        {
            lua_pop(_state, 1);
            break;
        }
        v.x = lua_tonumber(_state, -1);
        lua_pop(_state, 1);

        lua_getfield(_state, index, "y");
        if (!lua_isnumber(_state, -1))
        {
            lua_pop(_state, 1);
            break;
        }
        v.y = lua_tonumber(_state, -1);
        lua_pop(_state, 1);

        return Variant(v);
    }

    default:
        break;
    }

    return Variant();
}

void Lua::pushVariant(const Variant & variant)
{
    switch (variant.getType())
    {
    case Variant::TYPE_INT:
        lua_pushinteger(_state, variant.toInt());
        break;

    case Variant::TYPE_FLOAT:
        lua_pushnumber(_state, variant.toFloat());
        break;

    case Variant::TYPE_BOOL:
        lua_pushboolean(_state, variant.toBool());
        break;

    case Variant::TYPE_STRING:
        lua_pushstring(_state, variant.toString().c_str());
        break;

    case Variant::TYPE_VECTOR:
    {
        ut::Vector v = variant.toVector();
        lua_createtable(_state, 0, 2);
        lua_pushnumber(_state, v.x);
        lua_setfield(_state, -2, "x");
        lua_pushnumber(_state, v.y);
        lua_setfield(_state, -2, "y");
        break;
    }

    default:
        lua_pushnil(_state);
    }
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
