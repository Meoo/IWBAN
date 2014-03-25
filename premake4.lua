
-- Output directories option
newoption {
  trigger     = "output_directory",
  value       = "path",
  description = "[Default=bin] Output directory for the executable"
}

if not _OPTIONS["output_directory"] then
  _OPTIONS["output_directory"] = "bin"
end

-- GNU GProf option
newoption {
  trigger     = "gprof",
  value       = "bool",
  description = "[Default=false] Enable GNU Profiler file generation",
  allowed = {
    { "true",   "GNU Profiler enabled" },
    { "false",  "GNU Profiler disabled" },
  }
}

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

-- MwUtil configuration
dofile "make/MwUtil.lua"

-- Boost configuration
dofile "make/Boost.lua"

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

-- Enable profiling with gprof on GNU GCC
local GPROF         = bool_default(_OPTIONS["gprof"], false)

-- Enable static linking of stdlib
local STDLIB_STATIC = bool_default(_OPTIONS["stdlib_static"], false)

-- ///////////////////////////////////////////////////// --

solution "IWannaBeANinja"
  configurations { "Debug", "Release" }
  platforms { "native", "x32", "x64" }

  targetdir( BIN_DIR )
  objdir   ( OBJ_DIR )

  flags { "ExtraWarnings", "FatalWarnings", "NoRTTI" }
  
  if STDLIB_STATIC then
    flags { "StaticRuntime" }
  end
  
  if GPROF then
    flags { "Symbols" }
    configuration "GMake"
      buildoptions { "-pg" }
      linkoptions  { "-pg" }
  end

  configuration "GMake"
    buildoptions { "-std=c++11" }

  configuration "Debug"
    flags { "Symbols" }

  configuration "Release"
    flags   { "Optimize" }
    defines { "NDEBUG" }

-- ///////////////////////////////////////////////////// --

project "IWBAN"
  language "C++"
  location( MAKE_DIR )

  includedirs { "src", "tools" }

  files       { "src/**.cpp" }
  excludes    { "src/Global.cpp" }

  pchheader   "src/Global.hpp"
  pchsource   "src/Global.cpp"

  use_SFML    { "graphics", "audio", "window", "system" }
  use_Boost   { "iostreams", "system", "filesystem" }
  use_MwUtil  { }

  configuration "Debug"
    kind "ConsoleApp"

  configuration "Release"
    kind "WindowedApp"


-- ///////////////////////////////////////////////////// --

project "Packager"
  language "C++"
  location( MAKE_DIR )
  
  kind "ConsoleApp"
  targetname "pmk"

  includedirs { "tools" }

  files       { "tools/packager/**.cpp" }

  use_Boost   { "system", "filesystem" }
