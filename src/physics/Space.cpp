/**
 * @file   Space.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/Space.hpp>

namespace phy
{

Space::Space()
{
}

void Space::add(Body * body)
{
    IWBAN_PRE_PTR(body);
    IWBAN_PRE(body->getOwner());

    // TODO object->updateLastPosition(); ?
    auto ret = _bodies.insert(body);

    IWBAN_ASSERT(ret.second);
}

void Space::remove(Body * body)
{
    IWBAN_PRE_PTR(body);

    body->unlinkChilds();
    IWBAN_VERIFY(_bodies.erase(body));
}

void Space::refresh()
{
    // TODO Refresh quadtree
}

void Space::computePairs(const PairCallback & callback) const
{
    IWBAN_PRE(callback);

    // TODO Use QuadTree

    for (auto it = _bodies.begin(); it != _bodies.end(); ++it)
    {
        auto it2 = it;
        ++it2;
        for (; it2 != _bodies.end(); ++it2)
        {
            if (Body::canCollide(**it, **it2))
                callback(**it, **it2);
        }
    }
}

void Space::testRay(const ut::Vector & begin, const ut::Vector & end,
                    const RayCallback & callback) const
{
    IWBAN_PRE(callback);

    // TODO testRay
}

void Space::testRectangle(const ut::Rectangle & rect,
                          const RectangleCallback & callback) const
{
    IWBAN_PRE(callback);

    // TODO testRectangle
}

void Space::testShape(const Shape & shape,
                      const ShapeCallback & callback) const
{
    IWBAN_PRE(callback);

    // TODO testShape
    // Do not forget Body::canCollide
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
