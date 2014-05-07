/**
 * @file   Menu.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Menu.hpp>

namespace gui
{

Menu::Menu()
{
}

Menu::~Menu()
{
    for (Element * elem : _elements)
        delete elem;
}

void Menu::draw(gfx::DrawContext & context) const
{
    for (Element * elem : _elements)
    {
        // Draw every visible element
        elem->draw(context);
    }
}

void Menu::add(Element * element)
{
    BOOST_ASSERT(element && !element->isSelected());

    _elements.push_back(element);

    if (_selected == -1u && element->isSelectable())
    {
        _selected = _elements.size() - 1;
        element->select();
    }

    updateChilds();
}

void Menu::updateChilds()
{
    int y = 0;

    for (Element * elem : _elements)
    {
        // Update element position
        if (_centered)
        {
            int x = (getSize().x - elem->getSize().x) / 2;
            elem->setPosition(getPosition() + ut::Vector(x, y));
        }
        else
            elem->setPosition(getPosition() + ut::Vector(0, y));

        y += elem->getSize().y;

        if (_spacing)
        {
            y += _spacing;
            if (y > getSize().y)
                break;
        }
    }
}

void Menu::dispatchAction(sys::ActionId action)
{
    if (_selected == -1u)
        return;

    switch (action)
    {
    case sys::ACT_UP:
        for (unsigned i = _selected - 1; i != -1u; --i)
        {
            if (_elements[i]->isSelectable())
            {
                _elements[_selected]->deselect();
                _elements[i]->select();
                _selected = i;
                break;
            }
        }
        break;

    case sys::ACT_DOWN:
        for (unsigned i = _selected + 1; i < _elements.size(); ++i)
        {
            if (_elements[i]->isSelectable())
            {
                _elements[_selected]->deselect();
                _elements[i]->select();
                _selected = i;
                break;
            }
        }
        break;

    default:
        _elements[_selected]->dispatchAction(action);
        break;
    }

    updateChilds();
}

}
// namespace gui
