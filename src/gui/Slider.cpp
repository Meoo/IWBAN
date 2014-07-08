/**
 * @file   Slider.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Slider.hpp>

namespace gui
{

Slider::Slider()
{
    _border.setOutlineThickness(-3);
    _border.setOutlineColor(sf::Color(196, 196, 196));
    _border.setFillColor(sf::Color::Transparent);

    _center.setFillColor(sf::Color(128, 128, 128+32));

    updateValue();
}

Slider::Slider(const ut::Vector & size,
               int min, int max, int step, int value)
    : _size(size), _value(value), _value_min(min), _value_max(max), _step(step)
{
    _border.setSize(size);
    _border.setOutlineThickness(-3);
    _border.setOutlineColor(sf::Color(196, 196, 196));
    _border.setFillColor(sf::Color::Transparent);

    _center.setFillColor(sf::Color(128, 128, 128+32));

    updateValue();
}

void Slider::draw(gfx::GuiContext & context) const
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

ut::Rectangle Slider::getBounds() const
{
    return ut::Rectangle(_position.x, _position.y, _size.x, _size.y);
}

void Slider::dispatchAction(sys::ActionId action)
{
    if (action == sys::ACT_LEFT && _value > _value_min)
    {
        _value -= _step;

        if (_value < _value_min)
            _value = _value_min;

        if (_action)
            _action(_value);

        updateValue();
    }
    else if (action == sys::ACT_RIGHT && _value < _value_max)
    {
        _value += _step;

        if (_value > _value_max)
            _value = _value_max;

        if (_action)
            _action(_value);

        updateValue();
    }
}

void Slider::setSize(const ut::Vector & size)
{
    _size = size;
    _border.setSize(size);
    _border.setPosition(_position - _size / 2);
    _center.setPosition(_position - _size / 2);
}

void Slider::setPosition(const ut::Vector & position)
{
    _position = position;
    _border.setPosition(_position - _size / 2);
    _center.setPosition(_position - _size / 2);
}

void Slider::refresh()
{
    _center.setSize({
        (_size.x * (_value - _value_min)) / (_value_max - _value_min),
        _size.y
    });
}

void Slider::updateValue()
{
    _center.setSize({
        (_size.x * (_value - _value_min)) / (_value_max - _value_min),
        _size.y
    });
}

}
// namespace gui
