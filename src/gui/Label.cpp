/**
 * @file   Label.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Label.hpp>

#include <resources/Locale.hpp>

namespace gui
{

Label::Label(const std::string & string_key)
    : _text(res::getLocale().getString(string_key),
            res::getLocale().getFont())
{
}

void Label::draw(gfx::DrawContext & context) const
{
    sf::Text text(_text);
    text.setPosition(_position.x + _margin.x, _position.y + _margin.y);

    if (_shadow)
    {
        sf::Text shadow(text);

        shadow.move(2.5, 2.5);
        shadow.setColor(sf::Color(0, 0, 0, text.getColor().a / 2));
        context.draw(shadow);
    }

    context.draw(text);
}

ut::Vector Label::getSize() const
{
    sf::FloatRect rect = _text.getLocalBounds();
    return ut::Vector(rect.width, rect.height) + _margin * 2;
}

}
// namespace gui
