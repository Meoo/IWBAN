/**
 * @file   Projector.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <system/Controls.hpp>
#include <system/Projector.hpp>
#include <system/Screen.hpp>

namespace sys
{

Projector::Projector(Screen * screen)
    : _current_screen(screen)
{
    IWBAN_PRE_PTR(screen);

    _current_screen->show(this);
}

Projector::~Projector()
{
    _current_screen->hide();
}

void Projector::setScreen(Screen * screen)
{
    IWBAN_PRE_PTR(screen);

    if (_current_screen == screen) return;

    _current_screen->hide();
    _current_screen = screen;
    _current_screen->show(this);
}

void Projector::update(const sf::Time & update_time)
{
    // Update controls right before updating the screen
    getControls().update();

    _current_screen->onUpdate(update_time);

    // Check if we should change screen
    Screen * next_screen = _current_screen->getNextScreen();
    if (next_screen)
    {
        // Ignore if the next screen is the same as the current
        if (next_screen == _current_screen)
        {
            IWBAN_LOG_ERROR("Old and new screen are the same");
            _current_screen->resetNextScreen();
            return;
        }

        IWBAN_LOG_INFO("Changing screen");

        Screen * current = _current_screen;

        setScreen(next_screen);

        if (current->isDisposable())
            delete current;
        else
            current->resetNextScreen();
    }
}

void Projector::render(gfx::Renderer & renderer, const sf::Time & render_time) const
{
    _current_screen->onRender(renderer, render_time);
}

void Projector::mouseMove(const ut::Vector & position)
{
    _current_screen->onMouseMove(position);
}

void Projector::mouseClick(const ut::Vector & position)
{
    _current_screen->onMouseClick(position);
}

}
// namespace sys
