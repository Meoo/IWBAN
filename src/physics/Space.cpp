/**
 * @file   Space.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/Space.hpp>

#include <algorithm>
#include <vector>

namespace phy
{

Space::Space()
{
}

void Space::add(Body * body)
{
    IWBAN_PRE_PTR(body);

    // TODO object->updateLastPosition(); ?
#ifndef NDEBUG
    auto ret = _bodies.insert(body);
    IWBAN_ASSERT(ret.second);
#else
    _bodies.insert(body);
#endif
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

        std::vector<CollisionData> collisions;

        // Collision detection : Broad phase
        computePairs([this, &collisions]
                           (phy::Body & first_body,
                            phy::Body & second_body)
        {
            // Collision detection : Narrow phase
            CollisionData data;
            if (Body::collide(first_body, second_body, data))
                collisions.push_back(std::move(data));
        });

        // TODO Collision response
        for (const CollisionData & collision : collisions)
        {
            // We can const cast because we know we own them
            Body * first = const_cast<Body*>(collision.first);
            Body * secnd = const_cast<Body*>(collision.second);

            // TODO Compute strength a better way, and change strength variable name?
            if (first->getMass() == 0)
            {
                CollisionResult res_secnd;
                res_secnd.body      = first;
                res_secnd.body_position = first->_position;
                res_secnd.body_velocity = first->_velocity;
                res_secnd.mask      = collision.second_mask;
                res_secnd.origin    = collision.origin;
                res_secnd.normal    = - collision.normal;
                res_secnd.force     = - collision.mtv;
                res_secnd.strength  = 1;

                secnd->respond(res_secnd);
            }
            else if (secnd->getMass() == 0)
            {
                CollisionResult res_first;
                res_first.body      = secnd;
                res_first.body_position = secnd->_position;
                res_first.body_velocity = secnd->_velocity;
                res_first.mask      = collision.first_mask;
                res_first.origin    = collision.origin;
                res_first.normal    = collision.normal;
                res_first.force     = collision.mtv;
                res_first.strength  = 1;

                first->respond(res_first);
            }
            else
            {
                ut::Vector p = collision.mtv / 2;

                CollisionResult res_first;
                res_first.body      = secnd;
                res_first.body_position = secnd->_position;
                res_first.body_velocity = secnd->_velocity;
                res_first.mask      = collision.first_mask;
                res_first.origin    = collision.origin;
                res_first.normal    = collision.normal;
                res_first.force     = p;
                res_first.strength  = 0.5f;

                CollisionResult res_secnd;
                res_secnd.body      = first;
                res_secnd.body_position = first->_position;
                res_secnd.body_velocity = first->_velocity;
                res_secnd.mask      = collision.second_mask;
                res_secnd.origin    = collision.origin;
                res_secnd.normal    = - collision.normal;
                res_secnd.force     = - p;
                res_secnd.strength  = 0.5f;

                first->respond(res_first);
                secnd->respond(res_secnd);
            }
        }

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

    // Sort by x position
    std::vector<Body *> bodies(_bodies.begin(), _bodies.end());
    std::stable_sort(bodies.begin(), bodies.end(),
        [](Body * a, Body * b)
        {
            return (a->getBoundingBox().x) < (b->getBoundingBox().x);
        });

    for (auto it = bodies.begin(); it != bodies.end(); ++it)
    {
        ut::Rectangle abb = (*it)->getBoundingBox();
        float l = abb.x + abb.w;

        auto it2 = it;
        ++it2;
        for (; it2 != bodies.end(); ++it2)
        {
            // Stop when the right bound of
            ut::Rectangle bbb = (*it2)->getBoundingBox();
            if (bbb.x > l)
                break;

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
