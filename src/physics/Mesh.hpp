/**
 * @file   Mesh.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MESH_HPP_
#define _IWBAN_MESH_HPP_

#include <Global.hpp>

#include <utils/Rectangle.hpp>

namespace phy
{

class Mesh
{
public:
    // Virtual destructor
    virtual ~Mesh() {}

    // Virtual functions
    virtual const ut::Rectangle & getBounds() const = 0;

};
// class Mesh

}
// namespace phy

#endif // _IWBAN_MESH_HPP_
