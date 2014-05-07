/**
 * @file   Frame.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <gui/Frame.hpp>

namespace gui
{

Frame::Frame(const ut::Vector & size, Element * content)
    : _size(size), _content(content)
{
    BOOST_ASSERT(content);
    updateContent();
}

Frame::~Frame()
{
    delete _content;
}

void Frame::draw(gfx::DrawContext & context) const
{
    /* TODO Debug code
    sf::RectangleShape s(sf::Vector2f(_size.x, _size.y));
    s.setFillColor(sf::Color::Transparent);
    s.setOutlineColor(sf::Color::Red);
    s.setOutlineThickness(-1);
    s.setOrigin(0, 0);
    s.setPosition(_position.x, _position.y);
    context.draw(s);
    */

    _content->draw(context);
}

void Frame::setSize(const ut::Vector & size)
{
    _size = size;
    updateContent();
}

void Frame::setPosition(const ut::Vector & position)
{
    _position = position;
    updateContent();
}

void Frame::dispatchAction(sys::ActionId action)
{
    _content->dispatchAction(action);
    updateContent();
}

void Frame::updateContent()
{
    _content->setPosition(_position + (_size / 2) - (_content->getSize() / 2));
}

}
// namespace gui
