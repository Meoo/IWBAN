/**
 * @file   Simulator.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/CollisionData.hpp>
#include <physics/Shape.hpp>
#include <physics/Simulator.hpp>

namespace phy
{

Simulator::Simulator()
{
}

void Simulator::add(Object & object)
{
    _objects.push_back(object);
}

void Simulator::remove(const Object & object)
{
    _objects.erase(_objects.iterator_to(object));
}

void Simulator::step()
{
    for (Object & obj : _objects)
    {
        if (obj.isAwake())
            obj.step();
    }

    // TODO May be optimized with a bubble sort?
    _objects.sort(Object::Comparator());

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
}

}
// namespace phy
