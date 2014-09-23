/**
 * @file   AABoxMesh.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_AABOXMESH_HPP_
#define _IWBAN_AABOXMESH_HPP_

#include <Global.hpp>

#include <physics/Mesh.hpp>

namespace phy
{

class AABoxMesh : public Mesh
{
private:
    ut::Rectangle _box;

public:
            AABoxMesh() = default;

            AABoxMesh(ut::Rectangle box) : _box(box) {}

    void    setBounds(ut::Rectangle bounds)
    {
        _box = bounds;
    }

    const ut::Rectangle & getBounds() const override
    {
        return _box;
    }

};
// class Mesh

}
// namespace phy

#endif // _IWBAN_AABOXMESH_HPP_
