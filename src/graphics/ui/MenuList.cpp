/**
 * @file   MenuList.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/ui/MenuElement.hpp>
#include <graphics/ui/MenuList.hpp>

namespace gfx
{

void MenuList::draw(DrawContext & context)
{
    int y = 0;

    for (std::size_t i = _first_visible; i < _elements.size(); ++i)
    {
        MenuElement * elem = _elements[i];

        y += elem->getSize().y;
        if (y > getSize().y) break;

        if (_centered)
        {
            int x = (getSize().x - elem->getSize().x) / 2;
            elem->draw(context, getPosition() + ut::Vector(x, y));
        }
        else
            elem->draw(context, getPosition() + ut::Vector(0, y));
    }
}

void MenuList::add(MenuElement * element)
{
    _elements.push_back(element);
}

}
// namespace gfx
