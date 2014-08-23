
-- Output directories option
newoption {
  trigger     = "output_directory",
  value       = "path",
  description = "[Default=bin] Output directory for the executable"
}

if not _OPTIONS["output_directory"] then
  _OPTIONS["output_directory"] = "bin"
end

-- Static stdlib linkage option
newoption {
  trigger     = "stdlib_static",
  value       = "bool",
  description = "[Default=false] Enable static linking of standard libraries",
  allowed = {
    { "true",   "Static linking of stdlib enabled" },
    { "false",  "Static linking of stdlib disabled" },
  }
}

-- ///////////////////////////////////////////////////// --

-- SFML2 configuration
dofile "make/SFML.lua"

-- Boost configuration
dofile "make/Boost.lua"

-- Lua configuration
dofile "make/Lua.lua"

-- ///////////////////////////////////////////////////// --

local function bool_default(value, default)
  if not value then return default end
  if value:lower() == "true" then return true end
  if value:lower() == "false" then return false end
  error (value.." is not a valid boolean value")
end

-- Output directories
local BIN_DIR       = _OPTIONS["output_directory"]
local OBJ_DIR       = BIN_DIR .."/obj"
local MAKE_DIR      = BIN_DIR .."/make"

-- Enable static linking of stdlib
local STDLIB_STATIC = bool_default(_OPTIONS["stdlib_static"], false)

-- ///////////////////////////////////////////////////// --

solution "IWannaBeANinja"
  configurations { "Debug", "Release", "Profile" }
  platforms { "native", "x32", "x64" }

  targetdir( BIN_DIR )
  objdir   ( OBJ_DIR )

  flags   { "ExtraWarnings", "FatalWarnings", "NoRTTI" }
  defines { "'__USER__=\"$(USERNAME)\"'" }

  if STDLIB_STATIC then
    flags { "StaticRuntime" }
  end

  configuration "GMake"
    buildoptions { "-std=c++11" }

  configuration "Debug"
    flags   { "Symbols" }

  configuration "Release"
    flags   { "Optimize" }
    defines { "NDEBUG" }

  configuration "Profile"
    flags   { "Optimize", "Symbols" }
    defines { "NDEBUG" }

  configuration { "Profile", "GMake" }
    buildoptions { "-pg" }
    linkoptions  { "-pg" }

-- ///////////////////////////////////////////////////// --

project "IWBAN"
  language "C++"
  location( MAKE_DIR )

  kind "WindowedApp"

  includedirs { "src" }

  files       { "src/**.cpp" }
  excludes    { "src/Global.cpp" }

  pchheader   "src/Global.hpp"
  pchsource   "src/Global.cpp"

  use_SFML    { "graphics", "audio", "window", "system" }
  use_Boost   { "iostreams", "filesystem", "system" }
  use_Lua     {}

  -- Force WindowedApp on MacOSX to workaround XCode bug
  configuration { "Debug", "not MacOSX" }
    kind "ConsoleApp"

  -- Link time optimizations
  configuration { "Release", "GMake" }
    linkoptions { "-flto" }

  configuration { "Release", "VS*" }
    linkoptions { "/GL /LTCG" }


-- ///////////////////////////////////////////////////// --

project "Packager"
  language "C++"
  location( MAKE_DIR )

  kind "ConsoleApp"
  targetname "pkgtool"

  includedirs { "src", "tools" }

  files       { "tools/packager/**.cpp" }

  use_Boost   { "filesystem", "system" }

-- ///////////////////////////////////////////////////// --

project "MapCompiler"
  language "C++"
  location( MAKE_DIR )

  kind "ConsoleApp"
  targetname "mapc"

  includedirs { "tools" }

  files       { "tools/mapcompiler/**.cpp" }

  use_SFML    { "graphics", "window", "system" }
  use_Boost   { "filesystem", "system" }
