/**
 * @file   DrawContext.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/Settings.hpp>

#include <graphics/contexts/DrawContext.hpp>

namespace
{

#ifndef NDEBUG
const sf::Clock debug_bg_block;
#endif

}
// namespace

namespace gfx
{

DrawContext::DrawContext()
    : _open(false)
{
    _render_scene.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT);

    if (!cfg::pixelated)
        _render_scene.setSmooth(true);
}

void DrawContext::draw(const sf::Drawable & drawable,
                       const sf::RenderStates & states)
{
    IWBAN_PRE(_open);

    _render_scene.draw(drawable, states);
}

void DrawContext::fill(const sf::Color & color,
                       const sf::RenderStates & states)
{
    IWBAN_PRE(_open);

    sf::RectangleShape rect(sf::Vector2f(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT));
    rect.setFillColor(color);
    _render_scene.draw(rect, states);
}

void DrawContext::open()
{
    IWBAN_PRE(!_open);

#ifndef NDEBUG
    _render_scene.clear(sf::Color(
        std::abs(((debug_bg_block.getElapsedTime().asMilliseconds() / 31) % 510) - 255),
        std::abs(((debug_bg_block.getElapsedTime().asMilliseconds() / 43) % 510) - 255),
        std::abs(((debug_bg_block.getElapsedTime().asMilliseconds() / 59) % 510) - 255)));
#endif

    _open = true;
}

void DrawContext::close()
{
    IWBAN_PRE(_open);

    _render_scene.display();

    _open = false;
}

const sf::Texture & DrawContext::getTexture() const
{
    return _render_scene.getTexture();
}

}
// namespace gfx
