/**
 * @file   Choice.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Choice.hpp>

namespace gui
{

void Choice::enable()
{
    _enabled = true;
}

void Choice::disable()
{
    _enabled = false;
}

void Choice::select()
{
    Label::select();
    setTextColor(_selected_color);
}

void Choice::deselect()
{
    Label::deselect();
    setTextColor(_idle_color);
}

void Choice::dispatchAction(sys::ActionId action)
{
    if (action == sys::ACT_ACCEPT && _action)
        _action();
}

void Choice::setIdleColor(const sf::Color & color)
{
    _idle_color = color;
    if (isEnabled() && !isSelected())
        setTextColor(_idle_color);
}

void Choice::setSelectedColor(const sf::Color & color)
{
    _selected_color = color;
    if (isEnabled() && isSelected())
        setTextColor(_selected_color);
}

void Choice::setDisabledColor(const sf::Color & color)
{
    _disabled_color = color;
    if (!isEnabled())
        setTextColor(_disabled_color);
}

}
// namespace gui
