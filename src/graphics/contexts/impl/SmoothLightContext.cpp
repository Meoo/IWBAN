/**
 * @file   SmoothLightContext.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/DisplayConfig.hpp>
#include <config/Settings.hpp>

#include <graphics/contexts/impl/SmoothLightContext.hpp>

#include <resources/File.hpp>

namespace gfx
{

namespace impl
{

SmoothLightContext::SmoothLightContext()
    : _open(false)
{
    // TODO Use resource manager for shaders
    // Horizontal blur
    res::File frag_h = res::openFile("system/blur_h.glfs");
    std::string frag_h_str((const char*) frag_h.getData(), frag_h.getSize());

    // Vertical blur
    res::File frag_v = res::openFile("system/blur_v.glfs");
    std::string frag_v_str((const char*) frag_v.getData(), frag_v.getSize());

    if (_blur_h_filter.loadFromMemory(frag_h_str, sf::Shader::Fragment)
     && _blur_v_filter.loadFromMemory(frag_v_str, sf::Shader::Fragment))
    {
        _blur_h_filter.setParameter("texture", sf::Shader::CurrentTexture);
        _blur_h_filter.setParameter("blur_x", 1.f / IWBAN_FRAME_WIDTH);

        _blur_v_filter.setParameter("texture", sf::Shader::CurrentTexture);
        _blur_v_filter.setParameter("blur_y", 1.f / IWBAN_FRAME_HEIGHT);
    }
    else
        // TODO Should throw an exception? And maybe fall back to quick lighting
        // Using resource manager for shaders may resolve this issue
        IWBAN_LOG_ERROR("Failed to load light blur shader");

    _render_light.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT, true);
    _render_light_inter.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT);

    if (!cfg::pixelated)
        _render_light.setSmooth(true);
}

void SmoothLightContext::draw(const sf::Drawable & drawable)
{
    BOOST_ASSERT(_open);

    _render_light.draw(drawable, sf::RenderStates(sf::BlendAdd));
}

void SmoothLightContext::open(const sf::Color & ambient_light)
{
    BOOST_ASSERT(!_open);

    _render_light.clear(ambient_light);

    _open = true;
}

void SmoothLightContext::close()
{
    BOOST_ASSERT(_open);

    sf::RenderStates state(sf::BlendNone);

    // Horizontal blur
    _render_light.display();

    state.shader = &_blur_h_filter;
    _render_light_inter.draw(sf::Sprite(_render_light.getTexture()),
                             state);

    // Vertical blur
    _render_light_inter.display();

    state.shader = &_blur_v_filter;
    _render_light.draw(sf::Sprite(_render_light_inter.getTexture()),
                       state);

    _render_light.display();

    _open = false;
}

const sf::Texture & SmoothLightContext::getTexture() const
{
    return _render_light.getTexture();
}

}
// namespace impl

}
// namespace gfx
