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
    std::cout << _text.getLocalBounds().top << std::endl;
}

void Label::draw(gfx::DrawContext & context) const
{
    sf::Text text(_text);
    sf::FloatRect rect = text.getLocalBounds();

    // Cast to int then back to float to get pixel-aligned
    text.setPosition((int) (_position.x + _margin.x - rect.left),
                     (int) (_position.y + _margin.y - rect.top));

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

ut::Vector Label::getSize() const
{
    sf::FloatRect rect = _text.getLocalBounds();
    return ut::Vector(rect.width, rect.height) + _margin * 2;
}

}
// namespace gui
