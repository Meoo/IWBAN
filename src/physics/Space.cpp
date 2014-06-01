/**
 * @file   Simulator.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/CollisionData.hpp>
#include <physics/Shape.hpp>
#include <physics/Space.hpp>

namespace phy
{

Space::Space()
{
}

void Space::add(Object * object)
{
    object->updateLastPosition();
    _objects.push_back(*object);
}

void Space::remove(const Object * object)
{
    _objects.erase(_objects.iterator_to(*object));
}

void Space::step()
{
    for (Object & obj : _objects)
        obj.prepare();

    for (Object & obj : _objects)
        obj.step();

    // TODO Build QuadTree

    for (Object & obj : _objects)
    {
        if (!obj.isAwake())
            continue;

        // TODO Broad phase

        // TODO Get objects in QuadTree near obj

        for (Object & other : _objects)
        {
            if (&obj == &other)
                continue;

            // Narrow phase
            obj.collideWith(other);
        }
    }

    // Finish frame by saving last position
    for (Object & obj : _objects)
        obj.finish();
}

#ifndef NDEBUG
void Space::drawDebug(gfx::DebugContext & debug_context) const
{
    for (const Object & obj : _objects)
        obj.drawDebug(debug_context);
}
#endif

}
// namespace phy
