-- Variables:
--      MWUTIL_DIR
--      MWUTIL_INCLUDE_DIR

-- SFML2 options
newoption {
  trigger     = "mwutil_directory",
  value       = "path",
  description = "MwUtil base directory (must contain src directory)"
}

-- ///////////////////////////////////////////////////// --

local MWUTIL_DIR = _OPTIONS["mwutil_directory"] or ""

-- ///////////////////////////////////////////////////// --

if _OPTIONS["mwutil_directory"] then
  -- Check directory validity
  local m = os.matchfiles(MWUTIL_DIR .."/src/Mw/Config.hpp")
  if #m == 0 then
    print("Not a valid MwUtil directory : ".. MWUTIL_DIR)
  end
end

-- ///////////////////////////////////////////////////// --

-- Enable MwUtil
function use_MwUtil()
  includedirs { MWUTIL_DIR .."/src" }
end
