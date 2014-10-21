/**
 * @file   ConvexMesh.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CONVEXMESH_HPP_
#define _IWBAN_CONVEXMESH_HPP_

#include <Global.hpp>

#include <physics/Mesh.hpp>

#ifndef NDEBUG
#  include <SFML/Graphics/ConvexShape.hpp>
#endif

namespace phy
{

class ConvexMesh : public Mesh
{
private:
    ut::Rectangle   _bounds;

#ifndef NDEBUG
    sf::ConvexShape _shape;
#endif


public:
            ConvexMesh()
    {
#ifndef NDEBUG
        _shape.setFillColor(sf::Color::Transparent);
        _shape.setOutlineColor(sf::Color::White);
        _shape.setOutlineThickness(-2);
#endif
    }

    const ut::Rectangle & getBounds() const override
    {
        return _bounds;
    }

    Type getType() const override
    {
        return MESH_CONVEX;
    }

#ifndef NDEBUG
    void drawDebug(gfx::DebugContext & debug,
                   const sf::RenderStates & states) const override
    {
        debug.draw(_shape, states);
    }
#endif

};
// class ConvexMesh

}
// namespace phy

#endif // _IWBAN_CONVEXMESH_HPP_
