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

void Frame::select()
{
    Element::select();
    _content->select();
}

void Frame::deselect()
{
    Element::deselect();
    _content->deselect();
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
