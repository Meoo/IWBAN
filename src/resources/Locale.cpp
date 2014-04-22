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

/*const sf::String & getString(const char * id) const
{
    return _dict.get(id);
}

void loadFile(const std::string & filename)
{
    File file = openFile(filename);

    // Create stream
    std::istringstream stream;
    loc_st.rdbuf()->pubsetbuf(
        const_cast<char*>(file.getData()), file.getSize());

    boost::property_tree::info_parser::read_info(stream, _dict);
}*/

}
// namespace res
