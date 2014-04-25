/**
 * @file   Simulator.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SIMULATOR_HPP_
#define _IWBAN_SIMULATOR_HPP_

#include <Global.hpp>

#include <physics/Object.hpp>

#ifndef NDEBUG
#  include <graphics/contexts/DrawContext.hpp>
#endif

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
    void add(Object * object);
    void remove(const Object * object);

    void step();

    // TODO updateSingleObject ?

#ifndef NDEBUG
    void drawDebug(gfx::DrawContext & debug_context) const;
#endif

};
// class Simulator

}
// namespace phy

#endif // _IWBAN_SIMULATOR_HPP_
