/**
 * @file   AABoxMesh.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_AABOXMESH_HPP_
#define _IWBAN_AABOXMESH_HPP_

#include <Global.hpp>

#include <physics/Mesh.hpp>

#ifndef NDEBUG
#  include <SFML/Graphics/RectangleShape.hpp>
#endif

namespace phy
{

class AABoxMesh : public Mesh
{
private:
    ut::Rectangle       _box;

#ifndef NDEBUG
    sf::RectangleShape  _shape;
#endif


public:
            AABoxMesh()
    {
#ifndef NDEBUG
        _shape.setFillColor(sf::Color::Transparent);
        _shape.setOutlineColor(sf::Color::White);
        _shape.setOutlineThickness(-2);
#endif
    }

            AABoxMesh(const ut::Rectangle & box) : AABoxMesh()
    {
        setBounds(box);
    }

    void    setBounds(const ut::Rectangle & bounds)
    {
        _box = bounds;
#ifndef NDEBUG
        _shape.setOrigin(- _box.left, - _box.top);
        _shape.setSize(_box.getSize());
#endif
    }

    const ut::Rectangle & getBounds() const override
    {
        return _box;
    }

#ifndef NDEBUG
    void drawDebug(gfx::DebugContext & debug,
                   const sf::RenderStates & states) const override
    {
        debug.draw(_shape, states);
    }
#endif

};
// class Mesh

}
// namespace phy

#endif // _IWBAN_AABOXMESH_HPP_
