/**
 * @file   ScreenProjector.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <system/ScreenProjector.hpp>

#include <system/Screen.hpp>

namespace sys
{

ScreenProjector::ScreenProjector(Screen * screen)
    : _current_screen(screen)
{
    BOOST_ASSERT_MSG(screen, "Screen cannot be null");

    _current_screen->onShow();
}

ScreenProjector::~ScreenProjector()
{
    _current_screen->onHide();
}

void ScreenProjector::setScreen(Screen * screen)
{
    BOOST_ASSERT_MSG(screen, "Screen cannot be null");

    if (_current_screen == screen) return;

    _current_screen->onHide();
    _current_screen = screen;
    _current_screen->onShow();
}

void ScreenProjector::update()
{
    // TODO Update if screen is changed?
    _current_screen->onUpdate();
}

void ScreenProjector::render(gfx::Renderer & renderer) const
{
    _current_screen->onRender(renderer);
}

}
// namespace sys
