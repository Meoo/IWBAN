/**
 * @file   WorldScreen.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <system/screens/WorldScreen.hpp>

namespace sys
{

void WorldScreen::onUpdate()
{
    _world.update();
}

void WorldScreen::onRender(gfx::Renderer & renderer) const
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
