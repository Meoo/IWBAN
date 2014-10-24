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

LuaObject::LuaObject(lua_State * lua, LuaRegistry registry)
    : _lua(lua), _registry(registry)
{
    lua_rawgeti(_lua, LUA_REGISTRYINDEX, _registry);
    _index = lua_rawlen(_lua, -1) + 1;

    lua_newtable(_lua);
    lua_rawseti(_lua, -2, _index);

    lua_pop(_lua, 1);
}

LuaObject::~LuaObject()
{
    if (isValid())
    {
        // TODO Exception safety?
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, _registry);

        lua_pushnil(_lua);
        lua_rawseti(_lua, -2, _index);

        lua_pop(_lua, 1);
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
    runL(_state, script);
}

void Lua::runFile(const std::string & filename)
{
    runFileL(_state, filename);
}

LuaObject Lua::createObject(LuaRegistry registry)
{
    return createObjectL(_state, registry);
}

void Lua::pushObject(LuaObject & object)
{
    return pushObjectL(_state, object);
}

Variant Lua::toVariant(int index)
{
    return toVariantL(_state, index);
}

void Lua::pushVariant(const Variant & variant)
{
    pushVariantL(_state, variant);
}

bool Lua::pcall(int narg, int nres)
{
    return pcallL(_state, narg, nres);
}

// ---- ---- ---- ----

void Lua::runL(lua_State * l, const LuaScript & script)
{
    IWBAN_PRE(script.isValid());

    luaL_loadbuffer(l, script.getData(), script.getSize(), script.getSource());
    pcallL(l, 0, 0);
}

void Lua::runFileL(lua_State * l, const std::string & filename)
{
    {
        res::File f = res::openFile(filename);
        luaL_loadbuffer(l, (const char*) f.getData(), f.getSize(), filename.c_str());
    }
    pcallL(l, 0, 0);
}

LuaObject Lua::createObjectL(lua_State * l, LuaRegistry registry)
{
    return LuaObject(l, registry);
}

void Lua::pushObjectL(lua_State * l, LuaObject & object)
{
    if (object.isValid())
    {
        lua_rawgeti(l, LUA_REGISTRYINDEX, object.getRegistry());
        lua_rawgeti(l, -1, object.getIndex());
        lua_remove(l, -2);
    }
    else
        lua_pushnil(l);
}

Variant Lua::toVariantL(lua_State * l, int index)
{
    switch (lua_type(l, index))
    {
    case LUA_TNUMBER:
        return Variant((Variant::Float) lua_tonumber(l, index));

    case LUA_TBOOLEAN:
        return Variant(lua_toboolean(l, index));

    case LUA_TSTRING:
        return Variant(lua_tostring(l, index));

    case LUA_TTABLE:
    {
        // Vector (x and y in a table)
        ut::Vector v;

        lua_getfield(l, index, "x");
        if (!lua_isnumber(l, -1))
        {
            lua_pop(l, 1);
            break;
        }
        v.x = lua_tonumber(l, -1);
        lua_pop(l, 1);

        lua_getfield(l, index, "y");
        if (!lua_isnumber(l, -1))
        {
            lua_pop(l, 1);
            break;
        }
        v.y = lua_tonumber(l, -1);
        lua_pop(l, 1);

        return Variant(v);
    }

    default:
        break;
    }

    return Variant();
}

void Lua::pushVariantL(lua_State * l, const Variant & variant)
{
    switch (variant.getType())
    {
    case Variant::TYPE_INT:
        lua_pushinteger(l, variant.toInt());
        break;

    case Variant::TYPE_FLOAT:
        lua_pushnumber(l, variant.toFloat());
        break;

    case Variant::TYPE_BOOL:
        lua_pushboolean(l, variant.toBool());
        break;

    case Variant::TYPE_STRING:
        lua_pushstring(l, variant.toString().c_str());
        break;

    case Variant::TYPE_VECTOR:
    {
        ut::Vector v = variant.toVector();
        lua_createtable(l, 0, 2);
        lua_pushnumber(l, v.x);
        lua_setfield(l, -2, "x");
        lua_pushnumber(l, v.y);
        lua_setfield(l, -2, "y");
        break;
    }

    default:
        lua_pushnil(l);
    }
}

bool Lua::pcallL(lua_State * l, int narg, int nres)
{
    int ret = lua_pcall(l, narg, nres, 0);

    if (ret)
    {
        const char * msg = lua_tostring(l, -1);
        const char * what;
        switch (ret)
        {
        case LUA_ERRRUN: what = "Runtime";          break;
        case LUA_ERRMEM: what = "Memory";           break;
        case LUA_ERRERR: what = "Error handler";    break;
        default:         what = "Unknown";          break;
        }

        IWBAN_LOG_ERROR("Lua (%s): %s", what, msg);

        lua_pop(l, 1);
    }

    return ret == 0;
}

}
// namespace logic
