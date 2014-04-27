/**
 * @file   Locale.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <resources/File.hpp>
#include <resources/Locale.hpp>

//#include <boost/property_tree/info_parser.hpp>

#include <sstream>

namespace res
{

sf::String Locale::getString(const std::string & id) const
{
    Dictionnary::const_iterator it = _dict.find(id);
    if (it == _dict.end())
        return sf::String(IWBAN_MISSING_STRING);

    return sf::String::fromUtf8(it->second.begin(), it->second.end());
}

void Locale::loadFile(const std::string & filename)
{
    IWBAN_LOG_INFO("Loading dictionnary from '%s'\n", filename.c_str());

    File file = openFile(filename);

    // TODO Locale::loadFile
}

}
// namespace res
