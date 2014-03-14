/**
 * @file   Renderer.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/Renderer.hpp>

#include <config/Config.hpp>

#include <resources/File.hpp>

namespace
{

#ifndef NDEBUG
const sf::Clock debug_bg_block;
#endif

}
// namespace

namespace gfx
{

Renderer::Renderer(sf::RenderTarget & target)
    : _target(target)
{
    _render_scene.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT);
    _render_light.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT);

    // TODO Smooth enabled by default?
    //_render_scene.setSmooth(true);
    
    _smooth_light = false;

    // Smooth lightning
    if (cfg::smooth_light && sf::Shader::isAvailable())
    {
        // TODO Use resource manager for shaders
        // Pipeline vertex shader
        res::File vert = res::openFile("system/pipeline.glvs");
        std::string vert_str((const char*) vert.getData(), vert.getSize());

        // Horizontal blur
        res::File frag_h = res::openFile("system/blur_h.glfs");
        std::string frag_h_str((const char*) frag_h.getData(), frag_h.getSize());

        // Vertical blur
        res::File frag_v = res::openFile("system/blur_v.glfs");
        std::string frag_v_str((const char*) frag_v.getData(), frag_v.getSize());
        
        if (_blur_h_filter.loadFromMemory(vert_str, frag_h_str)
         && _blur_v_filter.loadFromMemory(vert_str, frag_v_str))
        {
            _blur_h_filter.setParameter("texture", sf::Shader::CurrentTexture);
            _blur_h_filter.setParameter("blur_x", 1.f / IWBAN_FRAME_WIDTH);
            
            _blur_v_filter.setParameter("texture", sf::Shader::CurrentTexture);
            _blur_v_filter.setParameter("blur_y", 1.f / IWBAN_FRAME_HEIGHT);

            _render_light_inter.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT);
            _smooth_light = true;
        }
        else
            IWBAN_LOG_WARNING("Failed to load blur shader, no smooth light\n");
    }

    IWBAN_DEBUG(_ready = false);
    IWBAN_DEBUG(_light_ready = false);
}

void Renderer::begin()
{
    BOOST_ASSERT_MSG(!_ready, "Renderer is already ready");
    IWBAN_DEBUG(_ready = true);

#ifndef NDEBUG
    _render_scene.clear(sf::Color(
        std::abs(((debug_bg_block.getElapsedTime().asMilliseconds() / 31) % 510) - 255),
        std::abs(((debug_bg_block.getElapsedTime().asMilliseconds() / 43) % 510) - 255),
        std::abs(((debug_bg_block.getElapsedTime().asMilliseconds() / 59) % 510) - 255)));
#endif
}

void Renderer::end()
{
    BOOST_ASSERT_MSG(_ready, "Renderer is not ready");
    IWBAN_DEBUG(_ready = false);

    _render_scene.display();
    _target.draw(sf::Sprite(_render_scene.getTexture()));
}

void Renderer::draw(const sf::Drawable & drawable,
                    const sf::RenderStates & states)
{
    BOOST_ASSERT_MSG(_ready, "Renderer is not ready");

    _render_scene.draw(drawable, states);
}

void Renderer::fill(const sf::Color & color,
                    const sf::RenderStates & states)
{
    BOOST_ASSERT_MSG(_ready, "Renderer is not ready");

    sf::RectangleShape rect(sf::Vector2f(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT));
    rect.setFillColor(color);
    _render_scene.draw(rect, states);
}

void Renderer::beginLight(const sf::Color & light_color)
{
    BOOST_ASSERT_MSG(_ready, "Renderer is not ready");
    BOOST_ASSERT_MSG(!_light_ready, "Renderer light is already ready");
    IWBAN_DEBUG(_light_ready = true);

    _render_light.clear(light_color);
}

void Renderer::endLight()
{
    BOOST_ASSERT_MSG(_ready, "Renderer is not ready");
    BOOST_ASSERT_MSG(_light_ready, "Renderer light is not ready");
    IWBAN_DEBUG(_light_ready = false);

    if (_smooth_light)
    {
        // Horizontal blur
        sf::RenderStates state(sf::BlendNone);
        state.shader = &_blur_h_filter;

        _render_light.display();
        _render_light_inter.draw(sf::Sprite(_render_light.getTexture()),
                                 state);

        // Vertical blur
        state.blendMode = sf::BlendMultiply;
        state.shader = &_blur_v_filter;

        _render_light_inter.display();
        _render_scene.draw(sf::Sprite(_render_light_inter.getTexture()),
                           state);
    }
    else
    {
        _render_light.display();
        _render_scene.draw(sf::Sprite(_render_light.getTexture()),
                           sf::RenderStates(sf::BlendMultiply));
    }
}

void Renderer::drawLight(const sf::Drawable & drawable)
{
    BOOST_ASSERT_MSG(_ready, "Renderer is not ready");
    BOOST_ASSERT_MSG(_light_ready, "Renderer light is not ready");

    _render_light.draw(drawable, sf::RenderStates(sf::BlendAdd));
}

}

// namespace gfx
