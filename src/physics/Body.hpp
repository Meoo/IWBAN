/**
 * @file   Body.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_BODY_HPP_
#define _IWBAN_BODY_HPP_

#include <Global.hpp>

#include <physics/Contact.hpp>
#include <physics/Space.hpp>

#include <utils/Vector.hpp>
#include <utils/Rectangle.hpp>

#include <boost/noncopyable.hpp>

#include <vector>

namespace phy
{

class Controller;
class Mesh;

class Body : public boost::noncopyable
{
public:
    typedef ut::Vector      Vector;


private:
    // Data members
    const Mesh *    _mesh;
    Controller *    _controller     = nullptr;

    Group           _group          = GROUP_NONE;
    Group           _collision_mask = GROUP_WORLD | GROUP_OBJECTS;

    Vector          _position;


public:
    // Constructor
    explicit        Body(const Mesh * mesh, Controller * controller = nullptr);

    // Getters / setters
    const Mesh *    getMesh() const                         { return _mesh; }

    Controller *    getController()                         { return _controller; }
    void            setController(Controller * controller)  { _controller = controller; }

    Group           getGroup() const                        { return _group; }
    void            setGroup(Group group)                   { _group = group; }

    Group           getCollisionMask() const                { return _collision_mask; }
    void            setCollisionMask(Group mask)            { _collision_mask = mask; }

    const Vector &  getPosition() const                     { return _position; }
    void            setPosition(const Vector & position)    { _position = position; }

    // TODO getBounds
    ut::Rectangle   getBounds() const;

    // Static function
    static
    void computeContacts(const Body & first, const Body & second,
                         std::vector<Contact> & output);

};
// class Body

}
// namespace phy

#endif // _IWBAN_BODY_HPP_
