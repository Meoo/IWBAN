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

void Space::add(Body * body)
{
    IWBAN_PRE_PTR(body);

    // TODO object->updateLastPosition(); ?
    auto ret = _bodies.insert(body);

    IWBAN_ASSERT(ret.second);
}

void Space::remove(Body * body)
{
    IWBAN_PRE_PTR(body);

    IWBAN_VERIFY(_bodies.erase(body));
}

void Space::step()
{
    /*for (Body & obj : _objects)
        obj.prepare();*/

    for (Body * body : _bodies)
        body->step();

    // TODO Build QuadTree

    /*for (Body & obj : _objects)
    {
        if (!obj.isAwake())
            continue;

        // TODO Broad phase

        // TODO Get objects in QuadTree near obj

        for (Body & other : _objects)
        {
            if (&obj == &other)
                continue;

            // Narrow phase
            obj.collideWith(other);
        }
    }*/

    // Finish frame by saving last position
    /*for (Body & obj : _objects)
        obj.finish();*/
}

#ifndef NDEBUG
void Space::drawDebug(gfx::DebugContext & debug_context) const
{
    for (const Body * body : _bodies)
        body->drawDebug(debug_context);
}
#endif

}
// namespace phy
