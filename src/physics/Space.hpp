/**
 * @file   Space.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SPACE_HPP_
#define _IWBAN_SPACE_HPP_

#include <Global.hpp>

#include <physics/Object.hpp>

#ifndef NDEBUG
#  include <graphics/contexts/debug/DebugContext.hpp>
#endif

namespace phy
{

class Space
{
public:
    // Data members
    Object::List    _objects;

    // Constructor
         Space();

    // Functions
    void add(Object * object);
    void remove(const Object * object);

    void step();

    // TODO updateSingleObject ?

#ifndef NDEBUG
    void drawDebug(gfx::DebugContext & debug_context) const;
#endif

};
// class Space

}
// namespace phy

#endif // _IWBAN_SPACE_HPP_
