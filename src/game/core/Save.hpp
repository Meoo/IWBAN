/**
 * @file   Save.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SAVE_HPP_
#define _IWBAN_SAVE_HPP_

#include <Global.hpp>

#include <logic/Lua.hpp>
#include <logic/Variant.hpp>

#include <boost/noncopyable.hpp>

#include <map>
#include <string>

namespace game
{

class LuaSave;

class Save
{
private:
    typedef std::string         String;
    typedef logic::Variant      Variant;
    typedef std::map<std::string, Variant> VariantMap;

    // Data members
    String      _filename;
    VariantMap  _values;


public:
                Save(const String & game_name, int save_number);

    void        load();
    void        save() const;

    Variant     getValue(const String & key) const;
    void        setValue(const String & key, const Variant & value);

    LuaSave     createLuaSave(logic::Lua & lua);

};
// class Save

// ---- ---- ---- ----

class LuaSave : public boost::noncopyable
{
private:
    typedef std::string         String;
    typedef logic::Variant      Variant;
    typedef logic::Lua          Lua;
    typedef logic::LuaObject    LuaObject;

    // Data members
    Save *          _save       = nullptr;
    LuaObject       _lua_obj;


public:
    // Default constructor
                    LuaSave() = default;

    // Move semantics
                    LuaSave(LuaSave && lua_save);
    LuaSave &       operator = (LuaSave && lua_save);

    // Getter
    LuaObject &     getLuaObject()  { return _lua_obj; }


private:
    // Constructor
    friend class Save;
                    LuaSave(Save * save, logic::Lua & lua);

    // Lua C functions
    static int      index(lua_State * l);
    static int      newIndex(lua_State * l);

};
// class LuaSave

}
// namespace game

#endif // _IWBAN_SAVE_HPP_
