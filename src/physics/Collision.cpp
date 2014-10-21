/**
 * @file   Collision.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/Body.hpp>
#include <physics/Mesh.hpp>
#include <physics/meshes/AABoxMesh.hpp>
#include <physics/meshes/BallMesh.hpp>
#include <physics/meshes/ConvexMesh.hpp>
#include <utils/Vector.hpp>

namespace
{

using phy::Body;
using phy::Contact;
using phy::Mesh;
using ut::Vector;

void computeContactsAABoxAABox(const Body & first, const Body & second,
                               const ut::Rectangle & inter,
                               std::vector<Contact> & output)
{
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

    if (contact.impulse.x != 0 && contact.impulse.y != 0)
    {
        if (std::abs(contact.impulse.x) < std::abs(contact.impulse.y))
            contact.impulse.y = 0;

        else
            contact.impulse.x = 0;
    }

    output.push_back(contact);
}
// computeContactsAABoxAABox()

void computeContactsBallBall(const Body & first, const Body & second,
                              std::vector<Contact> & output)
{
    const phy::BallMesh * ball1= reinterpret_cast<const phy::BallMesh *>(first.getMesh());
    const phy::BallMesh * ball2 = reinterpret_cast<const phy::BallMesh *>(second.getMesh());

    float r = ball1->getRadius() + ball2->getRadius();

    ut::Vector offset = second.getPosition() - first.getPosition();
    float l = offset.getLength();

    // No collision if the balls are not overlapping
    if (l >= r)
        return;

    Contact contact;

    // TODO const_cast ?
    contact.first   = const_cast<Body *>(&first);
    contact.second  = const_cast<Body *>(&second);

    // Special case if both balls have the same origin
    if (first.getPosition().x == second.getPosition().x
     && first.getPosition().y == second.getPosition().y)
    {
        contact.origin = first.getPosition();
    }
    else
    {
        contact.normal = ut::normalize(offset);
        contact.impulse = contact.normal * (r - l);
        contact.origin = first.getPosition()
                + contact.normal * (ball1->getRadius() + (r - l) / 2);
    }

    output.push_back(contact);
}
// computeContactsAABoxBall()

void computeContactsAABoxBall(const Body & first, const Body & second,
                              std::vector<Contact> & output)
{
    const ut::Rectangle box_bounds = first.getMesh()->getBounds();
    const phy::BallMesh * ball = reinterpret_cast<const phy::BallMesh *>(second.getMesh());

    ut::Vector offset = second.getPosition() - first.getPosition();
    float r = ball->getRadius();

    // Find the ball's center distance from the edges of the box
    float left = box_bounds.left - offset.x;
    if (left >= r) return;

    float right = offset.x - box_bounds.right;
    if (right >= r) return;

    float up = box_bounds.top - offset.y;
    if (up >= r) return;

    float down = offset.y - box_bounds.bottom;
    if (down >= r) return;

    Contact contact;

    // TODO const_cast ?
    contact.first   = const_cast<Body *>(&first);
    contact.second  = const_cast<Body *>(&second);

    // TODO contact.origin

    if (up >= down)
    {
        // Up
        if (left >= right)
        {
            // Up left
            if (up > 0 && left > 0)
            {
                // Corner
                ut::Vector c(left, up);
                float l = c.getLength();
                if (l > r)
                    return;

                contact.normal = c / l;
                contact.impulse = contact.normal * (r - l);
            }
            else if (up >= left)
            {
                contact.normal = Vector(0, -1);
                contact.impulse = Vector(0, r - up);
            }
            else // up < left
            {
                contact.normal = Vector(-1, 0);
                contact.impulse = Vector(r - left, 0);
            }
        }
        else
        {
            // Up right
            if (up > 0 && right > 0)
            {
                // Corner
                ut::Vector c(-right, up);
                float l = c.getLength();
                if (l > r)
                    return;

                contact.normal = c / l;
                contact.impulse = contact.normal * (r - l);
            }
            else if (up >= right)
            {
                contact.normal = Vector(0, -1);
                contact.impulse = Vector(0, r - up);
            }
            else // up < right
            {
                contact.normal = Vector(1, 0);
                contact.impulse = Vector(right - r, 0);
            }
        }
    }
    else // up < down
    {
        // Down
        if (left >= right)
        {
            // Down left
            if (down > 0 && left > 0)
            {
                // Corner
                ut::Vector c(left, -down);
                float l = c.getLength();
                if (l > r)
                    return;

                contact.normal = c / l;
                contact.impulse = contact.normal * (r - l);
            }
            else if (down >= left)
            {
                contact.normal = Vector(0, 1);
                contact.impulse = Vector(0, down - r);
            }
            else // down < left
            {
                contact.normal = Vector(-1, 0);
                contact.impulse = Vector(r - left, 0);
            }
        }
        else
        {
            // Down right
            if (down > 0 && right > 0)
            {
                // Corner
                ut::Vector c(-right, -down);
                float l = c.getLength();
                if (l > r)
                    return;

                contact.normal = c / l;
                contact.impulse = contact.normal * (r - l);
            }
            else if (down >= right)
            {
                contact.normal = Vector(0, 1);
                contact.impulse = Vector(0, down - r);
            }
            else // down < right
            {
                contact.normal = Vector(1, 0);
                contact.impulse = Vector(right - r, 0);
            }
        }
    }

    output.push_back(contact);
}
// computeContactsAABoxBall()

}
// namespace

// ---- ---- ---- ----

namespace phy
{

void computeContacts(const Body & first, const Body & second,
                     std::vector<Contact> & output)
{
    ut::Rectangle inter = first.getBounds().computeIntersection(second.getBounds());

    if (inter.isEmpty())
        return;

    switch (first.getMesh()->getType())
    {
    case Mesh::MESH_AABOX:
        switch (second.getMesh()->getType())
        {
        case Mesh::MESH_AABOX:
            computeContactsAABoxAABox(first, second, inter, output);
            return;

        case Mesh::MESH_BALL:
            computeContactsAABoxBall(first, second, output);
            return;

        case Mesh::MESH_CONVEX:
            break;
        }
        break;

    case Mesh::MESH_BALL:
        switch (second.getMesh()->getType())
        {
        case Mesh::MESH_AABOX:
            computeContactsAABoxBall(second, first, output);
            return;

        case Mesh::MESH_BALL:
            computeContactsBallBall(first, second, output);
            return;

        case Mesh::MESH_CONVEX:
            break;
        }
        break;

    case Mesh::MESH_CONVEX:
        switch (second.getMesh()->getType())
        {
        case Mesh::MESH_AABOX:
            break;

        case Mesh::MESH_BALL:
            break;

        case Mesh::MESH_CONVEX:
            break;
        }
        break;
    }

    IWBAN_LOG_ERROR("Unhandled collision ! (%d with %d)",
                    first.getMesh()->getType(),
                    second.getMesh()->getType());
}
// computeContacts()

}
// namespace phy
