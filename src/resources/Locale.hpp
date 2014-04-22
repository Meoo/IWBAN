/**
 * @file   Locale.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LOCALE_HPP_
#define _IWBAN_LOCALE_HPP_

#include <Global.hpp>

#include <boost/property_tree/ptree.hpp>

#include <SFML/System.hpp>

namespace res
{

class Locale
{
private:
    // TODO wstring instead of sf:String? and remove include?
    typedef boost::property_tree::basic_ptree<std::string, sf::String> Dictionnary;

    // Data members
    Dictionnary _dict;


public:

    // Functions
    const sf::String & getString(const char * id) const;

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
