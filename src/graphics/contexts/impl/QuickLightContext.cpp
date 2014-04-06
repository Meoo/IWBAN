/**
 * @file   QuickLightContext.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/contexts/impl/QuickLightContext.hpp>

namespace gfx
{

namespace impl
{

// TODO ready flag and asserts
QuickLightContext::QuickLightContext()
{
    _render_light.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT);
}

void QuickLightContext::draw(const sf::Drawable & drawable)
{
    _render_light.draw(drawable, sf::RenderStates(sf::BlendAdd));
}

void QuickLightContext::open(const sf::Color & ambient_light)
{
    _render_light.clear(ambient_light);
}

void QuickLightContext::close()
{
    _render_light.display();
}

const sf::Texture & QuickLightContext::getTexture() const
{
    return _render_light.getTexture();
}

}
// namespace impl

}
// namespace gfx
