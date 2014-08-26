/**
 * @file   ShadowVolume.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SHADOWVOLUME_HPP_
#define _IWBAN_SHADOWVOLUME_HPP_

#include <Global.hpp>

#include <utils/Rectangle.hpp>
#include <utils/Vector.hpp>

#include <SFML/Graphics.hpp>

#include <vector>

namespace gfx
{

class ShadowVolume
{
private:
    // Data members
    // Vertices must be listed clockwise
    // Last and first vertices are not considered as connected,
    // so they should be duplicates if the volume needs to be a closed polygon
    std::vector<ut::Vector> _vertices;

    ut::Vector              _position;

    sf::Color               _color      = sf::Color::White;


public:
    // Functions
    // TODO ? ut::Rectangle getBoundingBox() const;
    void dbgAddVertex(const ut::Vector & vec) { _vertices.push_back(vec); }

    // Accessors
    std::size_t         getVertexCount() const                      { return _vertices.size(); }
    ut::Vector          getVertex(std::size_t index) const          { return _position + _vertices[index]; }

    const ut::Vector &  getPosition() const                         { return _position; }
    void                setPosition(const ut::Vector & position)    { _position = position;}

    const sf::Color &   getShadowColor() const                      { return _color; }
    void                setShadowColor(const sf::Color & color)     { _color = color; }

};
// class ShadowVolume

}
// namespace gfx

#endif // _IWBAN_SHADOWVOLUME_HPP_
