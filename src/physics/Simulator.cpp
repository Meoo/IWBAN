/**
 * @file   Simulator.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

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

void Simulator::prepare()
{
    for (Object & obj : _objects)
    {
        obj.prepare();
    }
    _objects.sort(Object::Comparator());
}

void Simulator::update()
{
    // Perform collisions
    for (Object & obj : _objects)
    {
        if (!obj.isAwake())
            continue;

        obj.update();

        // Broad phase
        // TODO Slow because we have to compute broad phases for every object
        // ...

        // Narrow phase
        // ...
        {
            // Response
            // ...
        }
    }
}

void Simulator::updateSingleObject(Object & object) const
{
}

}
// namespace phy
