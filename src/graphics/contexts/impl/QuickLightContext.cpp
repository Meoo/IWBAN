/**
 * @file   QuickLightContext.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/Config.hpp>

#include <graphics/contexts/impl/QuickLightContext.hpp>

namespace gfx
{

namespace impl
{

QuickLightContext::QuickLightContext()
    : _open(false)
{
    _render_light.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT, true);

    if (!cfg::pixelated)
        _render_light.setSmooth(true);
}

void QuickLightContext::draw(const sf::Drawable & drawable)
{
    BOOST_ASSERT(_open);

    _render_light.draw(drawable, sf::RenderStates(sf::BlendAdd));
}

void QuickLightContext::open(const sf::Color & ambient_light)
{
    BOOST_ASSERT(!_open);

    _render_light.clear(ambient_light);

    _open = true;
}

void QuickLightContext::close()
{
    BOOST_ASSERT(_open);
 
    _render_light.display();

    _open = false;
}

const sf::Texture & QuickLightContext::getTexture() const
{
    return _render_light.getTexture();
}

}
// namespace impl

}
// namespace gfx
