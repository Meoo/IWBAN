/**
 * @file   Space.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <physics/Body.hpp>
#include <physics/Controller.hpp>
#include <physics/Space.hpp>

#ifndef NDEBUG
#  include <physics/Mesh.hpp>
#endif

namespace phy
{

void Space::attach(Body * body)
{
    IWBAN_PRE_PTR(body);

    auto it = _to_detach.find(body);

    if (it != _to_detach.end())
        _to_detach.erase(it);

    else
        _to_attach.insert(body);

}

void Space::detach(Body * body) noexcept
{
    IWBAN_PRE_PTR(body);

    auto it = _to_attach.find(body);

    if (it != _to_attach.end())
        _to_attach.erase(it);

    else
        _to_detach.insert(body);
}

void Space::update()
{
    // TODO Remove detached bodies
    for (Body * body : _to_attach)
        _bodies.insert(body);

    _to_attach.clear();

    // TODO Add newly attached bodies
    for (Body * body : _to_detach)
        _bodies.erase(body);

    _to_detach.clear();

    for (Body * body : _bodies)
    {
        if (Controller * ctrl = body->getController())
            ctrl->preUpdate(*this, *body);
    }

    // TODO Simulate a step
    for (unsigned step = 0; step < 4; ++step) // FIXME constant
    {
        for (Body * body : _bodies)
        {
            if (Controller * ctrl = body->getController())
                ctrl->step(*this, *body);
        }

        struct Pair { Body * first, * second; };
        std::vector<Pair> pairs;

        // TODO Broad phase : Find pairs that could be colliding
        // Actually : no broadphase, n^2 pairs with inactive bodies culled
        for (auto it = _bodies.begin(); it != _bodies.end(); ++it)
        {
            auto it2 (it);
            ++it2;
            for (; it2 != _bodies.end(); ++it2)
            {
                if (((*it)->getController() || (*it2)->getController())
                 && ((*it)->getBounds().isIntersecting((*it2)->getBounds())))
                    pairs.push_back({*it, *it2});
            }
        }

        std::vector<Contact> contacts;

        // TODO Narrow phase : Compute contacts
        for (const Pair & pair : pairs)
            Body::computeContacts(*(pair.first), *(pair.second), contacts);

        // TODO Resolution
        for (const Contact & contact : contacts)
        {
            if (Controller * ctrl = contact.first->getController())
                ctrl->resolveContact(*this, *contact.first, contact);

            if (Controller * ctrl = contact.second->getController())
                ctrl->resolveContact(*this, *contact.second,
                                     Contact::reverse(contact));
        }
    }

    for (Body * body : _bodies)
    {
        if (Controller * ctrl = body->getController())
            ctrl->postUpdate(*this, *body);
    }
}

#ifndef NDEBUG
void Space::drawDebug(gfx::DebugContext & debug) const
{
    sf::RenderStates r;
    for (Body * body : _bodies)
    {
        r.transform = sf::Transform::Identity;
        r.transform.translate(body->getPosition());
        body->getMesh()->drawDebug(debug, r);
    }
}
#endif

}
// namespace phy
