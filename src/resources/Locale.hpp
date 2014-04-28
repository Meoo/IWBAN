/**
 * @file   Locale.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LOCALE_HPP_
#define _IWBAN_LOCALE_HPP_

#include <Global.hpp>

#include <SFML/System.hpp>

#include <map>

namespace res
{

class Locale
{
private:
    typedef std::map<std::string, std::string> Dictionnary;

    // Data members
    Dictionnary _dict;


public:
    // Constructor
    Locale();

    // Functions
    sf::String getString(const std::string & key) const;

    void loadFile(const std::string & filename);

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
