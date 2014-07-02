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

void Space::update(const sf::Time & delta, int passes)
{
    IWBAN_PRE(delta.asSeconds() > 0);
    IWBAN_PRE(passes > 0);

    sf::Time step_delta = delta / sf::Int64(passes);

    // Pre update
    for (Body * body : _bodies)
        body->preUpdate(delta);

    for (int i = 0; i < passes; ++i)
    {
        // Pre step
        for (Body * body : _bodies)
            body->preStep(step_delta);

        // Collision detection : Broad phase
        computePairs([this](phy::Body & first_body,
                            phy::Body & second_body)
        {
            // Collision detection : Narrow phase
            phy::CollisionData data;
            if (phy::Body::collide(first_body, second_body, data))
            {
                // TODO Collision response
                if (data.first_mask != phy::COL_NONE)
                {
                }

                if (data.second_mask != phy::COL_NONE)
                {
                }
            }
        });

        // TODO Refresh quadtree

        // Post step
        for (Body * body : _bodies)
            body->postStep(step_delta);
    }

    // Post update
    for (Body * body : _bodies)
        body->postUpdate(delta);
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
