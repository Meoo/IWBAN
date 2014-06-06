/**
 * @file   WorldScreen.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <system/screens/WorldScreen.hpp>

namespace sys
{

void WorldScreen::onUpdate(const sf::Time & update_time)
{
    _world.update();
}

void WorldScreen::onRender(gfx::Renderer & renderer, const sf::Time & render_time) const
{
    _world.render(renderer);
}

void WorldScreen::onShow()
{
}

void WorldScreen::onHide()
{
}

}
// namespace sys
