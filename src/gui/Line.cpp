/**
 * @file   Line.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Line.hpp>

namespace gui
{

Line::Line(const std::vector<Element *> & content)
    : _content(content)
{
    for (Element * elem : _content)
        BOOST_ASSERT(elem);

    updateContent();
}

Line::~Line()
{
    for (Element * elem : _content)
        delete elem;
}

void Line::draw(gfx::DrawContext & context) const
{
    for (Element * elem : _content)
        elem->draw(context);
}

ut::Vector Line::getSize() const
{
    ut::Vector size;

    for (Element * elem : _content)
    {
        ut::Vector elemsize = elem->getSize();
        size.x += elemsize.x;
        if (elemsize.y > size.y)
            size.y = elemsize.y;
    }

    return size;
}

void Line::setPosition(const ut::Vector & position)
{
    _position = position;
    updateContent();
}

void Line::select()
{
    Element::select();

    for (Element * elem : _content)
        elem->select();
}

void Line::deselect()
{
    Element::deselect();

    for (Element * elem : _content)
        elem->deselect();
}

bool Line::isSelectable() const
{
    for (Element * elem : _content)
        if (elem->isSelectable())
            return true;

    return false;
}

void Line::dispatchAction(sys::ActionId action)
{
    for (Element * elem : _content)
        elem->dispatchAction(action);

    updateContent();
}

void Line::updateContent()
{
    ut::Vector size = getSize();
    ut::Vector cur;

    for (Element * elem : _content)
    {
        ut::Vector elemsize = elem->getSize();

        cur.y = (size.y - elemsize.y) / 2;
        elem->setPosition(_position + cur);
        cur.x += elemsize.x;
    }
}

}
// namespace gui
