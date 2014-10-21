/**
 * @file   Body.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/Body.hpp>
#include <physics/Mesh.hpp>

namespace phy
{

Body::Body(const Mesh * mesh, Controller * controller)
    : _mesh(mesh), _controller(controller)
{
    IWBAN_PRE_PTR(mesh);
}

Body::~Body()
{
    if (_space)
        _space->remove(this);
}

ut::Rectangle Body::getBounds() const
{
    if (!_mesh)
        return ut::Rectangle();

    ut::Rectangle bounds = _mesh->getBounds();
    bounds.translate(getPosition());
    return bounds;
}

void Body::init(Space * space, Owner * owner)
{
    IWBAN_PRE_PTR(space);
    IWBAN_PRE_PTR(owner);
    IWBAN_PRE(!_space);

    _space = space;
    _owner = owner;
}

void Body::deinit()
{
    IWBAN_PRE_PTR(_space);

    _space = nullptr;
    _owner = nullptr;
}

}
// namespace phy

