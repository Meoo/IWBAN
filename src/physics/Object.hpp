/**
 * @file   Object.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OBJECT_HPP_
#define _IWBAN_OBJECT_HPP_

#include <Global.hpp>

#include <physics/CollisionGroup.hpp>

#include <utils/Vector.hpp>

#include <boost/intrusive/list.hpp>

namespace phy
{

class Behavior;
class Shape;

class Object : public boost::intrusive::list_base_hook<>
{
public:
    class Comparator;
    typedef boost::intrusive::list<Object> List;


private:
    // Data members
    Shape *         _shape;

    ut::Vector      _position;
    ut::Vector      _last_position;

    Behavior *      _behavior;
    void *          _user_data;

    CollisionGroup  _solidity;
    CollisionGroup  _collides_with;

    int             _collision_priority;
    bool            _awake;

    // TODO Object additional states
    //Object * parent;
    //bool just_teleported;
    //bool smooth;


public:
    // Constructor
            Object();

    // Functions
    void    move(const ut::Vector & delta);
    void    teleport(const ut::Vector & position);

    // Getters / setters TODO Bad collision priority get/set name
    int     getPriority() const         { return _collision_priority; }
    void    setPriority(int priority)   { _collision_priority = priority; }

    bool    isAwake() const             { return _awake; }
    void    sleep()                     { _awake = false; }
    void    wake()                      { _awake = true; }


    class Comparator
    {
    public:
        bool operator () (const Object & a, const Object & b) const
            { return a.getPriority() < b.getPriority(); }
    };
    // class Comparator

};
// class Object

}
// namespace phy

#endif // _IWBAN_OBJECT_HPP_
