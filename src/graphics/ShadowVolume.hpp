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

#include <boost/noncopyable.hpp>

#include <vector>

namespace gfx
{

class Scene;

class ShadowVolume : public boost::noncopyable
{
private:
    // Data members
    Scene *                 _scene      = nullptr;

    // Vertices must be listed clockwise
    // Last and first vertices are not considered as connected,
    // so they should be duplicates if the volume needs to be a closed polygon
    std::vector<ut::Vector> _vertices;

    ut::Vector              _position;

    ut::Rectangle           _bounds;

    // Colors that are filtered by this volume
    // Default is White : no light can pass through the volume
    sf::Color               _color      = sf::Color::White;


public:
    // Destructor
                        ~ShadowVolume();

    // Functions

    // FIXME Debug function
    void                dbgAddVertex(const ut::Vector & vec)        { _vertices.push_back(vec); }

    // Accessors
    std::size_t         getVertexCount() const                      { return _vertices.size(); }
    ut::Vector          getVertex(std::size_t index) const          { return _position + _vertices[index]; }

    const ut::Vector &  getPosition() const                         { return _position; }
    void                setPosition(const ut::Vector & position)    { _position = position;}

    const sf::Color &   getShadowColor() const                      { return _color; }
    void                setShadowColor(const sf::Color & color)     { _color = color; }

    ut::Rectangle       getBounds() const                           { auto b = _bounds; b.translate(_position); return b; }


private:
    friend class Scene;

    Scene *     getScene()                  { return _scene; }

    void        init(Scene * scene);
    void        deinit();

};
// class ShadowVolume

}
// namespace gfx

#endif // _IWBAN_SHADOWVOLUME_HPP_
