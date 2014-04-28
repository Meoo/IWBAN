/**
 * @file   Projector.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <system/Projector.hpp>

#include <system/Screen.hpp>

namespace sys
{

Projector::Projector(Screen * screen)
    : _current_screen(screen)
{
    BOOST_ASSERT_MSG(screen, "Screen cannot be null");

    _current_screen->onShow();
}

Projector::~Projector()
{
    _current_screen->onHide();
}

void Projector::setScreen(Screen * screen)
{
    BOOST_ASSERT_MSG(screen, "Screen cannot be null");

    if (_current_screen == screen) return;

    _current_screen->onHide();
    _current_screen = screen;
    _current_screen->onShow();
    _current_screen->setNextScreen(0);
}

void Projector::update()
{
    _current_screen->onUpdate();

    Screen * next_screen = _current_screen->getNextScreen();
    if (next_screen)
    {
        BOOST_ASSERT(next_screen != _current_screen);

        Screen * current = _current_screen;

        setScreen(next_screen);

        if (current->isDisposable())
            delete current;
    }
}

void Projector::render(gfx::Renderer & renderer) const
{
    _current_screen->onRender(renderer);
}

}
// namespace sys
