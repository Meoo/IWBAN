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

// TODO Real computeContacts implementation
void Body::computeContacts(const Body & first, const Body & second,
                           std::vector<Contact> & output)
{
    ut::Rectangle inter = first.getBounds().computeIntersection(second.getBounds());

    if (inter.isEmpty())
        return;

    Contact contact;

    // TODO const_cast ?
    contact.first   = const_cast<Body *>(&first);
    contact.second  = const_cast<Body *>(&second);

    contact.origin = (Vector(inter.left, inter.top)
                    + Vector(inter.right, inter.bottom)) / 2;

    bool fleft = first.getBounds().left == inter.left;
    bool fright = first.getBounds().right == inter.right;
    bool ftop = first.getBounds().top == inter.top;
    bool fbot = first.getBounds().bottom == inter.bottom;

    if (fleft && !fright)
        contact.impulse.x = inter.right - inter.left;

    else if (fright && !fleft)
        contact.impulse.x = inter.left - inter.right;

    if (ftop && !fbot)
        contact.impulse.y = inter.bottom - inter.top;

    else if (fbot && !ftop)
        contact.impulse.y = inter.top - inter.bottom;

    if (!contact.impulse.isNull())
        contact.normal = ut::normalize(contact.impulse);

    output.push_back(contact);
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

