/**
 * @file   BallMesh.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BALLMESH_HPP_
#define _IWBAN_BALLMESH_HPP_

#include <Global.hpp>

#include <physics/Mesh.hpp>

#ifndef NDEBUG
#  include <SFML/Graphics/CircleShape.hpp>
#endif

namespace phy
{

class BallMesh : public Mesh
{
private:
    ut::Rectangle   _bounds;
    float           _radius;

#ifndef NDEBUG
    sf::CircleShape _shape;
#endif


public:
            BallMesh() : _radius(0)
    {
#ifndef NDEBUG
        _shape.setPointCount(16);
        _shape.setFillColor(sf::Color::Transparent);
        _shape.setOutlineColor(sf::Color::White);
        _shape.setOutlineThickness(-2);
#endif
    }

            BallMesh(float radius) : BallMesh()
    {
        setRadius(radius);
    }

    float   getRadius() const
    {
        return _radius;
    }

    void    setRadius(float radius)
    {
        _radius         = radius;
        _bounds.left    = - radius;
        _bounds.top     = - radius;
        _bounds.right   = radius;
        _bounds.bottom  = radius;

#ifndef NDEBUG
        _shape.setRadius(radius);
        _shape.setOrigin(radius, radius);
#endif
    }

    const ut::Rectangle & getBounds() const override
    {
        return _bounds;
    }

    Type getType() const override
    {
        return MESH_BALL;
    }

#ifndef NDEBUG
    void drawDebug(gfx::DebugContext & debug,
                   const sf::RenderStates & states) const override
    {
        debug.draw(_shape, states);
    }
#endif

};
// class BallMesh

}
// namespace phy

#endif // _IWBAN_BALLMESH_HPP_
