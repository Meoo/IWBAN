/**
 * @file   Slider.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Slider.hpp>

namespace gui
{

Slider::Slider(const ut::Vector & size, unsigned value, unsigned max_value)
    : _size(size), _value(value), _value_max(max_value)
{
    _border.setSize(size);
    _border.setOutlineThickness(-3);
    _border.setOutlineColor(sf::Color(196, 196, 196));
    _border.setFillColor(sf::Color::Transparent);

    _center.setFillColor(sf::Color(128, 128, 128+32));

    updateValue();
}

void Slider::draw(gfx::DrawContext & context) const
{
    if (_shadow)
    {
        sf::RectangleShape shadow(_size);
        shadow.setPosition(_border.getPosition() + ut::Vector(2.5, 2.5));
        shadow.setFillColor(sf::Color(0, 0, 0, 255/2));

        context.draw(shadow);
    }
    context.draw(_center);
    context.draw(_border);
}

void Slider::select()
{
    _border.setOutlineColor(sf::Color::White);
    _center.setFillColor(sf::Color(128, 128, 128+96));
}

void Slider::deselect()
{
    _border.setOutlineColor(sf::Color(159, 159, 159));
    _center.setFillColor(sf::Color(128, 128, 128+32));
}

void Slider::dispatchAction(sys::ActionId action)
{
    if (action == sys::ACT_LEFT && _value > 0)
    {
        --_value;
        updateValue();
    }
    else if (action == sys::ACT_RIGHT && _value < _value_max)
    {
        ++_value;
        updateValue();
    }
}

void Slider::setPosition(const ut::Vector & position)
{
    _border.setPosition(position);
    _center.setPosition(position);
}

void Slider::updateValue()
{
    _center.setSize({
        (_size.x * _value) / _value_max,
        _size.y
    });
}

}
// namespace gui