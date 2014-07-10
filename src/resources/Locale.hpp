/**
 * @file   Locale.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LOCALE_HPP_
#define _IWBAN_LOCALE_HPP_

#include <Global.hpp>

#include <resources/File.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <map>
#include <set>

namespace res
{

class Locale
{
private:
    typedef std::map<std::string, std::string> Dictionnary;
    typedef std::set<std::string> FileList;

    // Data members
    Dictionnary _dict;
    FileList    _files;

    // Font file must be kept open
    File        _default_font_file;
    sf::Font    _default_font;


public:
    // Constructor
    Locale();

    // Functions
    const sf::Font & getFont() const { return _default_font; }

    sf::String getString(const std::string & key) const;

    void loadFont(const std::string & filename);

    void loadFile(const std::string & filename);

    // Reload all files
    void reload();

};
// class Locale

// ---- ---- ---- ----

inline
Locale & getLocale()
{
    static Locale s_locale;
    return s_locale;
}

}
// namespace res

#endif // _IWBAN_LOCALE_HPP_
