/**
 * @file   Map.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MAP_HPP_
#define _IWBAN_MAP_HPP_

#include <Global.hpp>

namespace game
{

class Map
{
private:
    // Data members
    sf::Color   _ambient_light;


public:
    // Use white to disable lighting
    const sf::Color & getAmbientLight() const { return _ambient_light; }

};
// class Map

}
// namespace game

#endif // _IWBAN_MAP_HPP_
