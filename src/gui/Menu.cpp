/**
 * @file   Menu.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Menu.hpp>

namespace gui
{

void Menu::draw(gfx::DrawContext & context)
{
    int y = 0;

    for (std::size_t i = _first_visible; i < _elements.size(); ++i)
    {
        Element * elem = _elements[i];

        y += elem->getSize().y;
        if (y > getSize().y)
            break;

        if (_centered)
        {
            int x = (getSize().x - elem->getSize().x) / 2;
            elem->draw(context, getPosition() + ut::Vector(x, y));
        }
        else
            elem->draw(context, getPosition() + ut::Vector(0, y));

        if (_spacing)
        {
            y += _spacing;
            if (y > getSize().y)
                break;
        }
    }
}

void Menu::add(Element * element)
{
    BOOST_ASSERT(element);

    _elements.push_back(element);
}

}
// namespace gui
