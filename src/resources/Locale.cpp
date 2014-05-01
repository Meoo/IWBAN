/**
 * @file   Locale.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <resources/FileStream.hpp>
#include <resources/Locale.hpp>

#include <locale>

namespace res
{

Locale::Locale()
{
    // Set global locale in order to get wide strings working
    std::locale::global(std::locale(""));
}

sf::String Locale::getString(const std::string & key) const
{
    Dictionnary::const_iterator it = _dict.find(key);
    if (it == _dict.end())
    {
        IWBAN_LOG_ERROR("Missing key '%s' from dictionnary\n", key.c_str());
        return sf::String(IWBAN_MISSING_STRING);
    }

    return sf::String::fromUtf8(it->second.begin(), it->second.end());
}

void Locale::loadFont(const std::string & filename)
{
    _default_font_file = openFile(filename);
    if (!_default_font.loadFromMemory(_default_font_file.getData(),
                                      _default_font_file.getSize()))
    {
        IWBAN_LOG_ERROR("Unable to load font '%s'\n", filename.c_str());
    }
}

void Locale::loadFile(const std::string & filename)
{
    File file = openFile(filename);
    FileStream istr(file);

    istr >> std::ws;
    while (!istr.eof())
    {
        std::string id;
        std::string value;

        // Read ASCII string identifier
        istr >> id >> std::ws;
        if (istr.eof())
        {
            IWBAN_LOG_ERROR("Unexpected EOF after key '%s' in '%s'\n",
                    id.c_str(), filename.c_str());
            return;
        }

        // Read UTF-8 string value
        char separator = istr.get();
        if (!std::strchr("\"'", separator))
        {
            IWBAN_LOG_ERROR("Invalid separator '%c' for key '%s' in '%s'",
                    separator, id.c_str(), filename.c_str());
            return;
        }

        // TODO This is really bad...?
        char c = istr.get();
        while(c != separator)
        {
            value += c;
            c = istr.get();

            if (istr.eof())
            {
                IWBAN_LOG_ERROR("Unexpected EOF while waiting for '%c' for key '%s' in '%s'",
                        separator, id.c_str(), filename.c_str());
                return;
            }
        }

        _dict[id] = value;

        istr >> std::ws;
    }
}
// Locale::loadFile()

}
// namespace res
