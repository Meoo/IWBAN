/**
 * @file   Button.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Button.hpp>

#include <resources/Locale.hpp>

namespace gui
{

Button::Button()
{
    _text.setFont(res::getLocale().getFont());
    setTextColor(_idle_color);
}

Button::Button(const std::string & string_key)
    : _key(string_key),
      _text(res::getLocale().getString(string_key),
            res::getLocale().getFont())
{
    setTextColor(_idle_color);
}

void Button::draw(gfx::GuiContext & context) const
{
    sf::Text text(_text);
    sf::FloatRect rect = text.getLocalBounds();
    unsigned sz = text.getCharacterSize();

    // Cast to int then back to float to get pixel-aligned
    text.setPosition((int) (_position.x - rect.width / 2 - rect.left),
                     (int) (_position.y - sz / 2 - rect.top));

    if (_shadow)
    {
        sf::Text shadow(text);

        // We are not pixel aligned, but the smooth effect produced is cool for shadows
        shadow.move(2.5, 2.5);
        shadow.setColor(sf::Color(0, 0, 0, text.getColor().a / 2));
        context.draw(shadow);
    }

    context.draw(text);
}


void Button::enable()
{
    _enabled = true;
    setTextColor(_idle_color);
}

void Button::disable()
{
    _enabled = false;
    setTextColor(_disabled_color);
}

void Button::select()
{
    _selected = true;
    setTextColor(_selected_color);
}

void Button::deselect()
{
    _selected = false;
    setTextColor(_idle_color);
}

ut::Rectangle Button::getBounds() const
{
    sf::Text text(_text);
    sf::FloatRect rect = text.getLocalBounds();
    unsigned sz = text.getCharacterSize();

    text.setPosition((int) (_position.x - rect.width / 2 - rect.left),
                     (int) (_position.y - sz / 2 - rect.top));

    return text.getGlobalBounds();
}

void Button::dispatchAction(sys::ActionId action)
{
    if (action == sys::ACT_ACCEPT && _action)
        _action();
}

void Button::setText(const std::string & string_key)
{
    _key = string_key;
    _text.setString(res::getLocale().getString(string_key));
}

void Button::setIdleColor(const sf::Color & color)
{
    _idle_color = color;

    if (_enabled && !_selected)
        setTextColor(_idle_color);
}

void Button::setSelectedColor(const sf::Color & color)
{
    _selected_color = color;

    if (_enabled && _selected)
        setTextColor(_selected_color);
}

void Button::setDisabledColor(const sf::Color & color)
{
    _disabled_color = color;

    if (!_enabled)
        setTextColor(_disabled_color);
}

void Button::refresh()
{
    if (!_key.empty())
        _text.setString(res::getLocale().getString(_key));

    if (_enabled)
    {
        if (_selected)
            setTextColor(_selected_color);
        else
            setTextColor(_idle_color);
    }
    else
        setTextColor(_disabled_color);
}

}
// namespace gui
