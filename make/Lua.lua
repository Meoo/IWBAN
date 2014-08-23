-- Functions:
--      use_Lua()

-- Lua options
newoption {
  trigger     = "lua_directory",
  value       = "path",
  description = "Lua base directory (must contain include directory)"
}

-- ///////////////////////////////////////////////////// --

local LUA_DIR         = _OPTIONS["lua_directory"] or ""
local LUA_INCLUDE_DIR = LUA_DIR .."/include"
local LUA_LIBS_DIR    = LUA_DIR .."/lib"

-- ///////////////////////////////////////////////////// --

-- Lua check
if LUA_DIR ~= "" then
  local m = os.matchfiles(LUA_INCLUDE_DIR .."/luaconf.h")
  if #m == 0 then
    print("Not a valid Lua include directory : ".. LUA_INCLUDE_DIR)
  end

  local m = os.matchfiles(LUA_LIBS_DIR .."/liblua.a")
  if #m == 0 then
    print("Lua library not found in directory : ".. LUA_LIBS_DIR)
  end
end

-- ///////////////////////////////////////////////////// --

-- Enable Lua
function use_Lua()
  -- Lua paths
  includedirs { LUA_INCLUDE_DIR }
  libdirs     { LUA_LIBS_DIR }

  links "lua"
end
