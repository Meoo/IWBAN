-- Functions:
--      use_SFML(libs)

-- SFML2 options
newoption {
  trigger     = "sfml_directory",
  value       = "path",
  description = "SFML2 base directory (must contain include and extlibs directory)"
}

newoption {
  trigger     = "sfml_bin_directory",
  value       = "path",
  description = "[Optional] SFML2 build directory (must contains lib directory)"
}

if not _OPTIONS["sfml_bin_directory"] then
  _OPTIONS["sfml_bin_directory"] = _OPTIONS["sfml_directory"] or ""
end

newoption {
  trigger     = "sfml_static",
  value       = "bool",
  description = "[Default=false] Link SFML2 as a static library",
  allowed = {
    { "true",   "Static linking enabled" },
    { "false",  "Static linking disabled" },
  }
}

-- ///////////////////////////////////////////////////// --

local function bool_default(value, default)
  if not value then return default end
  if value:lower() == "true" then return true end
  if value:lower() == "false" then return false end
  error (value.." is not a valid boolean value")
end

local SFML_DIR      = _OPTIONS["sfml_directory"] or ""
local SFML_STATIC   = bool_default(_OPTIONS["sfml_static"], false)
local SFML_LIBS_DIR = _OPTIONS["sfml_bin_directory"] .. "/lib"

-- ///////////////////////////////////////////////////// --

if _OPTIONS["sfml_directory"] ~= "" then
  -- Check directory validity
  local m = os.matchfiles(SFML_DIR .."/include/SFML/Config.hpp")
  if #m == 0 then
    print("Not a valid SFML directory : ".. SFML_DIR)
  end

  local m = os.matchfiles(SFML_LIBS_DIR .."/libsfml-*")
  if #m == 0 then
    print("No SFML libraries in directory : ".. SFML_LIBS_DIR)
  end
end

-- ///////////////////////////////////////////////////// --

-- Enable SFML
function use_SFML(libs)
  -- SFML paths
  includedirs { SFML_DIR .."/include" }
  libdirs     { SFML_LIBS_DIR }

  -- Flags
  if SFML_STATIC then
    defines { "SFML_STATIC" }
  else
    defines { "SFML_DYNAMIC" }
  end

  -- SFML libs
  local s = ""
  if SFML_STATIC then
    s = "-s"
  end

  configuration "Debug"
    for k, v in pairs(libs) do
        if v ~= "main" then
            links("sfml-" .. v .. s .. "-d")
        else
            links("sfml-" .. v .. "-d")
        end
    end

  configuration "Release"
    for k, v in pairs(libs) do
        if v ~= "main" then
            links("sfml-" .. v .. s)
        else
            links("sfml-" .. v)
        end
    end

  -- External libs paths
  -- Will cause warnings when linking if you do not specify a platform
  configuration { "VS*", "native" }
    libdirs { SFML_DIR .."/extlibs/libs-msvc/x86/" }
    libdirs { SFML_DIR .."/extlibs/libs-msvc/x64/" }

  configuration { "VS*", "x32" }
    libdirs { SFML_DIR .."/extlibs/libs-msvc/x86/" }

  configuration { "VS*", "x64" }
    libdirs { SFML_DIR .."/extlibs/libs-msvc/x64/" }

  configuration { "Windows", "GMake", "native" }
    libdirs { SFML_DIR .."/extlibs/libs-mingw/x86/" }
    libdirs { SFML_DIR .."/extlibs/libs-mingw/x64/" }

  configuration { "Windows", "GMake", "x32" }
    libdirs { SFML_DIR .."/extlibs/libs-mingw/x86/" }

  configuration { "Windows", "GMake", "x64" }
    libdirs { SFML_DIR .."/extlibs/libs-mingw/x64/" }

  -- External libs
  configuration "Windows"
    links { "glew32", "opengl32", "gdi32", "openal32", "sndfile", "winmm", "freetype", "jpeg" }

  configuration "Linux"
    links { "GL", "GLEW", "X11", "Xrandr", "pthread", "freetype", "jpeg" }

  configuration {}
end
