/**
 * @file   Object.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OBJECT_HPP_
#define _IWBAN_OBJECT_HPP_

#include <Global.hpp>

#include <physics/CollisionGroup.hpp>
#include <physics/Shape.hpp>
#include <physics/ObjectState.hpp>

#include <boost/intrusive/list.hpp>

namespace phy
{

class Behavior;

class Object : public boost::intrusive::list_base_hook<>
{
public:
    class Comparator;
    typedef boost::intrusive::list<Object> List;


private:
    // Data members
    Shape             shape;

    ObjectState       current;
    ObjectState       previous;

    CollisionGroup    solidity;
    CollisionGroup    collides_with;

    int               _collision_priority;
    Behavior *        controller;
    bool              awake;

    // TODO Object additional states
    //bool              smooth;


public:
    // Constructor
            Object();

    // Getters / setters TODO Bad collision priority get/set name
    int     getPriority() const         { return _collision_priority; }
    void    setPriority(int priority)   { _collision_priority = priority; }


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
