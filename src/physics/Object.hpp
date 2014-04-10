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

namespace impl
{
    class ChildTag {};
}
// namespace impl

class Behavior;
class Shape;

// Child-list support auto-unlink feature
class Object : public boost::intrusive::list_base_hook<>
             , public boost::intrusive::list_base_hook<
                    boost::intrusive::tag<impl::ChildTag>,
                    boost::intrusive::link_mode<boost::intrusive::auto_unlink> >
{
public:
    class Comparator;

    typedef boost::intrusive::list<Object>      List;

    typedef boost::intrusive::list<Object,
        boost::intrusive::tag<impl::ChildTag> > ChildList;


private:
    // Data members
    Shape *         _shape;

    ChildList       _childs;

    ut::Vector      _position;
    ut::Vector      _last_position;

    Behavior *      _behavior;
    void *          _user_data;

    CollisionGroup  _solidity;
    CollisionGroup  _collides_with;

    int             _collision_priority;
    bool            _awake;

    // TODO Object additional states
    //bool just_teleported;
    //bool smooth;


public:
    // Constructor
            Object();

    // Functions
    void    move(const ut::Vector & delta);
    void    teleport(const ut::Vector & position);

    void    prepare();
    void    update();

    void    setParent(Object & parent);
    void    unsetParent();
    // TODO getParent()? not required?

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
