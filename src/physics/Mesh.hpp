/**
 * @file   Mesh.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MESH_HPP_
#define _IWBAN_MESH_HPP_

#include <Global.hpp>

#include <utils/Rectangle.hpp>

#ifndef NDEBUG
#  include <graphics/contexts/debug/DebugContext.hpp>
#endif

namespace phy
{

class Mesh
{
public:
    enum Type
    {
        MESH_AABOX,
        MESH_BALL,
        MESH_CONVEX,
    };


    // Virtual destructor
    virtual         ~Mesh() {}

    // Virtual functions
    virtual const ut::Rectangle & getBounds() const = 0;

    virtual Type    getType() const = 0;

#ifndef NDEBUG
    virtual void    drawDebug(gfx::DebugContext & debug,
                           const sf::RenderStates & states) const = 0;
#endif

};
// class Mesh

}
// namespace phy

#endif // _IWBAN_MESH_HPP_
