/**
 * @file   Label.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Label.hpp>

#include <data/Locale.hpp>

namespace gui
{

Label::Label()
{
    _text.setFont(data::getLocale().getFont());
}

Label::Label(const std::string & string_key)
    : _key(string_key),
      _text(data::getLocale().getString(string_key),
            data::getLocale().getFont())
{
}

void Label::draw(gfx::GuiContext & context) const
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

void Label::loadText(const std::string & string_key)
{
    _key = string_key;
    _text.setString(data::getLocale().getString(string_key));
}

void Label::refresh()
{
    if (!_key.empty())
        _text.setString(data::getLocale().getString(_key));
}

}
// namespace gui
