/**
 * @file   DebugContext.cpp
 * @author Bastien Brunnenstein
 */

#ifndef NDEBUG

#include <Global.hpp>

#include <config/DisplayConfig.hpp>
#include <config/Settings.hpp>

#include <graphics/contexts/debug/DebugContext.hpp>

namespace gfx
{

DebugContext::DebugContext()
    : _open(false)
{
    _render_scene.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT);

    if (!cfg::pixelated)
        _render_scene.setSmooth(true);
}

void DebugContext::draw(const sf::Drawable & drawable,
                       const sf::RenderStates & states)
{
    BOOST_ASSERT(_open);

    _render_scene.draw(drawable, states);
}

void DebugContext::fill(const sf::Color & color,
                       const sf::RenderStates & states)
{
    BOOST_ASSERT(_open);

    sf::RectangleShape rect(sf::Vector2f(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT));
    rect.setFillColor(color);
    _render_scene.draw(rect, states);
}

void DebugContext::open(const sf::View & view)
{
    BOOST_ASSERT(!_open);

    _render_scene.setView(view);
    _render_scene.clear(sf::Color(0, 0, 0, 0));

    _open = true;
}

void DebugContext::close()
{
    BOOST_ASSERT(_open);

    _render_scene.display();

    _open = false;
}

const sf::Texture & DebugContext::getTexture() const
{
    return _render_scene.getTexture();
}

}
// namespace gfx

#endif // NDEBUG
