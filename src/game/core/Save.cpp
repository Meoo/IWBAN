/**
 * @file   Save.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

// TODO Remove that... must find another way to fix boost's bug
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <config/PathsConfig.hpp>
#include <game/core/Save.hpp>
#include <utils/StreamIO.hpp>

#include <boost/filesystem.hpp>

#include <fstream>
#include <sstream>

namespace game
{

Save::Save(const String & game_name, int save_number)
{
    std::ostringstream s;
    s << game_name << save_number;
    _filename = s.str();
}

void Save::load()
{
    namespace fs = boost::filesystem;

    fs::path path(IWBAN_SAVES_FOLDER);

    if (!fs::is_directory(path))
    {
        IWBAN_LOG_INFO("Failed to load save '%s' : File not found", _filename.c_str());
        return;
    }

    // Append filename to folder
    path /= _filename;

    if (!fs::exists(path))
    {
        IWBAN_LOG_INFO("Failed to load save '%s' : File not found", _filename.c_str());
        return;
    }

    // FIXME Load
    // TODO Unzip / Decrypt ?
    std::ifstream istr(path.string());

    istr.exceptions(std::ios::eofbit
                  | std::ios::failbit
                  | std::ios::badbit);

    std::size_t count = ut::read<std::size_t>(istr);

    for (std::size_t i = 0; i < count; ++i)
    {
        String s = ut::read<String>(istr);
        Variant v = ut::read<Variant>(istr);

        _values[s] = v;
    }
}

void Save::save() const
{
    namespace fs = boost::filesystem;

    fs::path path(IWBAN_SAVES_FOLDER);

    if (!fs::is_directory(path))
        fs::create_directory(path);

    // Append filename to folder
    path /= _filename;

    // Backup handling
    if (fs::exists(path))
    {
        fs::path backup(path);
        backup += ".old";

        if (fs::exists(backup))
            fs::remove(backup);

        fs::rename(path, backup);
    }

    // FIXME Save
    // TODO Zip / Encrypt ?
    std::ofstream ostr(path.string());

    ostr.exceptions(std::ios::eofbit
                  | std::ios::failbit
                  | std::ios::badbit);

    ut::write<std::size_t>(ostr, _values.size());

    for (auto it : _values)
    {
        ut::write<String>(ostr, it.first);
        ut::write<Variant>(ostr, it.second);
    }

    ostr.flush();
    ostr.close();
}

Save::Variant Save::getValue(const String & key) const
{
    const auto it = _values.find(key);

    if (it == _values.end())
        return Variant();

    return it->second;
}

void Save::setValue(const String & key, const Variant & value)
{
    auto it = _values.find(key);

    if (it != _values.end())
    {
        if (value.isNull())
            _values.erase(it);

        else
            it->second = value;
    }
    else if (!value.isNull())
    {
        _values[key] = value;
    }
}

LuaSave Save::createLuaSave(logic::Lua & lua)
{
    return LuaSave(this, lua);
}

// ---- ---- ---- ----

LuaSave::LuaSave(LuaSave && lua_save)
    : _save(lua_save._save), _lua_obj(std::move(lua_save._lua_obj))
{
}

LuaSave & LuaSave::operator = (LuaSave && lua_save)
{
    _save = lua_save._save;
    _lua_obj = std::move(lua_save._lua_obj);

    return *this;
}

LuaSave::LuaSave(Save * save, logic::Lua & lua)
    : _save(save)
{
    _lua_obj = lua.createObject(logic::LUA_IWBAN_SPECIAL);

    lua.pushObject(_lua_obj);

    // __index
    lua_pushlightuserdata(lua, this);
    lua_pushcclosure(lua, index, 1);
    lua_setfield(lua, -2, "__index");

    // __newindex
    lua_pushlightuserdata(lua, this);
    lua_pushcclosure(lua, newIndex, 1);
    lua_setfield(lua, -2, "__newindex");

    // Set itself as metatable
    lua.pushObject(_lua_obj);
    lua_setmetatable(lua, -2);

    lua_pop(lua, 1);
}

int LuaSave::index(lua_State * l)
{
    LuaSave * lthis = static_cast<LuaSave *>(lua_touserdata(l, lua_upvalueindex(1)));

    luaL_checktype(l, 1, LUA_TTABLE);

    String key (luaL_checkstring(l, 2));

    Lua::pushVariantL(l, lthis->_save->getValue(key));

    return 1;
}

int LuaSave::newIndex(lua_State * l)
{
    LuaSave * lthis = static_cast<LuaSave *>(lua_touserdata(l, lua_upvalueindex(1)));

    luaL_checktype(l, 1, LUA_TTABLE);

    String key (luaL_checkstring(l, 2));
    Variant value = Lua::toVariantL(l, 3);

    lthis->_save->setValue(key, value);

    return 0;
}

}
// namespace game
