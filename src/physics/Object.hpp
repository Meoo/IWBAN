/**
 * @file   Object.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_OBJECT_HPP_
#define _IWBAN_OBJECT_HPP_

#include <Global.hpp>

#include <boost/intrusive/list.hpp>

namespace phy
{

class Object : public boost::intrusive::list_base_hook<>
{
private:
    // Data members
    CollisionGroup    my_groups;
    CollisionGroup    col_groups;
    Shape             shape;
    float             scale;
    int               collision_priority;
    ObjectState       current;
    ObjectState       last;
    Behavior *        controller;
    bool              awake;
    bool              smooth;

};
// class Object

}
// namespace phy

#endif // _IWBAN_OBJECT_HPP_
