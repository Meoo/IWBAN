-- Functions:
--      use_Boost(libs)

-- For this script to work correctly, compile boost with
-- bjam option "--layout=tagged"

-- Boost options
newoption {
  trigger     = "boost_directory",
  value       = "path",
  description = "Boost base directory (must contain boost directory)"
}

newoption {
  trigger     = "boost_bin_directory",
  value       = "path",
  description = "[Optional] Boost build directory (must contains lib directory)"
}

newoption {
  trigger     = "boost_runtime_static",
  value       = "bool",
  description = "[Default=false] Use Boost with standard library linked statically",
  allowed = {
    { "true",   "Static runtime linking enabled" },
    { "false",  "Static runtime linking disabled" },
  }
}

-- ///////////////////////////////////////////////////// --

-- Read options
local function bool_default(value, default)
  if not value then return default end
  if value:lower() == "true" then return true end
  if value:lower() == "false" then return false end
  error (value.." is not a valid boolean value")
end

local BOOST_DIR            = _OPTIONS["boost_directory"] or ""
local BOOST_INCLUDE_DIR    = BOOST_DIR
local BOOST_LIBS_DIR       = (_OPTIONS["boost_bin_directory"]
                           or BOOST_DIR .. "/stage") .."/lib"
local BOOST_RUNTIME_STATIC = bool_default(_OPTIONS["boost_runtime_static"], false)

-- ///////////////////////////////////////////////////// --

-- Boost check
if BOOST_DIR ~= "" then
  local m = os.matchfiles(BOOST_INCLUDE_DIR .."/boost/config.hpp")
  if #m == 0 then
    print("Not a valid Boost include directory : ".. BOOST_INCLUDE_DIR)
  else

    local m = os.matchfiles(BOOST_LIBS_DIR .."/libboost_*")
    if #m == 0 then
      print("No a valid Boost libraries directory : ".. BOOST_LIBS_DIR)
    end
  end
end

-- ///////////////////////////////////////////////////// --

-- Enable Boost
function use_Boost(libs)
  -- Boost paths
  includedirs { BOOST_INCLUDE_DIR }
  libdirs     { BOOST_LIBS_DIR }

  -- Disable deprecated features
  defines     { "BOOST_SYSTEM_NO_DEPRECATED", "BOOST_FILESYSTEM_NO_DEPRECATED" }

  -- Boost libs
  local s = ""
  if BOOST_RUNTIME_STATIC then
    s = "s"
  end

  local ABI = function(str)
    if str ~= "" then
      return "-" .. str
    else
      return ""
    end
  end

  configuration "Debug"
    for k, v in pairs(libs) do
      if v ~= "main" then
        links("boost_" .. v .. "-mt" .. ABI(s .. "d"))
      end
    end

  configuration "Release"
    for k, v in pairs(libs) do
      if v ~= "main" then
        links("boost_" .. v .. "-mt" .. ABI(s))
      end
    end

  configuration {}
end

