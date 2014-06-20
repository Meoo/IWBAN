/**
 * @file   Selector.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Selector.hpp>

#include <resources/Locale.hpp>

namespace gui
{

void Selector::addEntry(int value, const std::string & string_key)
{
    // TODO Selector addEntry
}

void Selector::select()
{
    Label::select();
    setTextColor(_selected_color);
}

void Selector::deselect()
{
    Label::deselect();
    setTextColor(_idle_color);
}

void Selector::dispatchAction(sys::ActionId action)
{
    // TODO Selector dispatch action
}

void Selector::setIdleColor(const sf::Color & color)
{
    _idle_color = color;
    updateColor();
}

void Selector::setSelectedColor(const sf::Color & color)
{
    _selected_color = color;
    updateColor();
}

void Selector::updateColor()
{
    if (isSelected())
        setTextColor(_selected_color);
    else
        setTextColor(_idle_color);
}

}
// namespace gui
