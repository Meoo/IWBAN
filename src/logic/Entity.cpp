/**
 * @file   Entity.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <logic/Entity.hpp>
#include <logic/World.hpp>

namespace logic
{

Entity::Entity()
{
    getWorld().add(this);
}

Entity::~Entity()
{
    getWorld().remove(this);
}

// FIXME Debug
void Entity::testRender(gfx::DrawContext & draw)
{
    for (gfx::Drawable * drawable : _drawables)
        draw.draw(*drawable);
}

void Entity::addDrawable(gfx::Drawable * drawable)
{
    _drawables.insert(drawable);
}

void Entity::removeDrawable(gfx::Drawable * drawable)
{
    _drawables.erase(drawable);
}

void Entity::addBody(phy::Body * body)
{
    _bodies.insert(body);
    getWorld().getSpace().attach(body);
}

void Entity::removeBody(phy::Body * body)
{
    getWorld().getSpace().detach(body);
    _bodies.erase(body);
}

}
// namespace logic
