/**
 * @file   GuiContext.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/contexts/GuiContext.hpp>

namespace
{

#ifndef NDEBUG
const sf::Clock debug_bg_block;
#endif

}
// namespace

namespace gfx
{

GuiContext::GuiContext(sf::RenderTarget & target)
    : _render_target(target), _open(false)
{
}

void GuiContext::draw(const sf::Drawable & drawable,
                       const sf::RenderStates & states)
{
    IWBAN_PRE(_open);

    _render_target.draw(drawable, states);
}

void GuiContext::open()
{
    IWBAN_PRE(!_open);

    _open = true;
}

void GuiContext::close()
{
    IWBAN_PRE(_open);

    _open = false;
}

}
// namespace gfx
