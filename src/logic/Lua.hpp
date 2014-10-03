/**
 * @file   Lua.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LUA_HPP_
#define _IWBAN_LUA_HPP_

#include <Global.hpp>

#include <lua.hpp>

#include <string>

namespace logic
{

class LuaScript
{
private:
    std::string     _source;
    std::string     _buffer;


public:
    // Constructors
    LuaScript() = default;

    LuaScript(const std::string & source, const std::string & buffer)
        : _source(source), _buffer(buffer) {}

    LuaScript(const std::string & source, const char * data, std::size_t size)
        : _source(source), _buffer(data, size) {}

    // Static constructor
    static LuaScript fromFile(const std::string & filename);


private:
    friend class Lua;

    const char *    getSource() const { return _source.c_str(); }
    const char *    getData() const { return _buffer.c_str(); }
    std::size_t     getSize() const { return _buffer.size(); }

};
// class LuaScript

// ---- ---- ---- ----

class Lua
{
private:
    lua_State * _state;


public:
    // Constructor
                Lua();

    // Destructor
                ~Lua();

    // Conversion to lua_State allow to use Lua object in lua_ functions
    operator    lua_State*() { return _state; }

    // Functions
    void        run(const LuaScript & script, const char * source);
    void        runFile(const std::string & filename);

    // Return false if the call returned in error
    bool        pcall(int narg, int nres);

};
// class logic

}
// namespace log

#endif // _IWBAN_LUA_HPP_
