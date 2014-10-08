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
                if (((*it)->getController() || (*it2)->getController()))
                    if (((*it)->getGroup() & (*it2)->getCollisionMask())
                     || ((*it2)->getGroup() & (*it)->getCollisionMask()))
                        if (((*it)->getBounds().isIntersecting((*it2)->getBounds())))
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
            if (contact.first->getCollisionMask() & contact.second->getGroup())
                if (Controller * ctrl = contact.first->getController())
                    ctrl->resolveContact(*this, *contact.first, contact);

            if (contact.second->getCollisionMask() & contact.first->getGroup())
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

Body * Space::tracePoint(const Vector & point, Group group_mask) const
{
    // TODO Optimize

    for (Body * body : _bodies)
        if (group_mask & body->getGroup())
            if (body->getBounds().isContaining(point))
                return body;

    return nullptr;
}

std::vector<Body *> Space::tracePointM(const Vector & point, Group group_mask) const
{
    // TODO Optimize

    std::vector<Body *> res;

    for (Body * body : _bodies)
        if (group_mask & body->getGroup())
            if (body->getBounds().isContaining(point))
                res.push_back(body);

    return res;
}

Body * Space::traceRect(const ut::Rectangle & rect, Group group_mask) const
{
    // TODO Optimize

    for (Body * body : _bodies)
        if (group_mask & body->getGroup())
            if (body->getBounds().isIntersecting(rect))
                return body;

    return nullptr;
}

std::vector<Body *> Space::traceRectM(const ut::Rectangle & rect, Group group_mask) const
{
    // TODO Optimize

    std::vector<Body *> res;

    for (Body * body : _bodies)
        if (group_mask & body->getGroup())
            if (body->getBounds().isIntersecting(rect))
                res.push_back(body);

    return res;
}

Contact Space::traceBody(const Body & body) const
{
    // TODO Optimize

    std::vector<Contact> res;

    for (Body * other : _bodies)
    {
        if (body.getCollisionMask() & other->getGroup())
        {
            Body::computeContacts(body, *other, res);

            if (!res.empty())
                return res[0];
        }
    }

    return Contact();
}

std::vector<Contact> Space::traceBodyM(const Body & body) const
{
    // TODO Optimize

    std::vector<Contact> res;

    for (Body * other : _bodies)
        if (body.getCollisionMask() & other->getGroup())
            Body::computeContacts(body, *other, res);

    return res;
}

Contact Space::traceBody(const Body & body, Group group_mask) const
{
    // TODO Optimize

    std::vector<Contact> res;

    for (Body * other : _bodies)
    {
        if (group_mask & other->getGroup())
        {
            Body::computeContacts(body, *other, res);

            if (!res.empty())
                return res[0];
        }
    }

    return Contact();
}

std::vector<Contact> Space::traceBodyM(const Body & body, Group group_mask) const
{
    // TODO Optimize

    std::vector<Contact> res;

    for (Body * other : _bodies)
        if (group_mask & other->getGroup())
            Body::computeContacts(body, *other, res);

    return res;
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
