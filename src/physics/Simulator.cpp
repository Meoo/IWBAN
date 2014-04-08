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
    _objects.remove(object);
}

void Simulator::update()
{
    // Preparation
    for (Object & obj : _objects)
    {
        // obj.last = obj.current
        // obj.controller.step(obj.last, obj.current)
    }
    for (Object & obj : _objects)
    {
        // if (obj.parent)
        {
            // obj.current + delta(obj.parent.current / last)
        }
    }
    _objects.sort(Object::Comparator());

    // We can run particles updates in another thread by now
    // ...

    // Narrow phase
    // ...

    // Broad phase
    // ...

    // Response
    // ...
}

void Simulator::updateSingleObject(Object & object) const
{
}

}
// namespace phy
