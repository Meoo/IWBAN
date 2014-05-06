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

void Label::draw(gfx::DrawContext & context, const ut::Vector & origin)
{
    _text.setPosition(origin.x + _margin.x, origin.y + _margin.y);
    context.draw(_text);
}

ut::Vector Label::getSize() const
{
    sf::FloatRect rect = _text.getLocalBounds();
    return ut::Vector(rect.width, rect.height) + _margin * 2;
}

}
// namespace gui
