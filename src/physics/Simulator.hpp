/**
 * @file   Simulator.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SIMULATOR_HPP_
#define _IWBAN_SIMULATOR_HPP_

#include <Global.hpp>

#include <physics/Object.hpp>

namespace phy
{

class Simulator
{
public:
    // Data members
    Object::List    _objects;

    // Constructor
         Simulator();

    // Functions
    void add(Object & object);
    void remove(const Object & object);

    void prepare();
    void update();
    void updateSingleObject(Object & object) const;

};
// class Simulator

}
// namespace phy

#endif // _IWBAN_SIMULATOR_HPP_
