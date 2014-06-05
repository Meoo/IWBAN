/**
 * @file   Renderer.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/Renderer.hpp>
#include <graphics/contexts/impl/QuickLightContext.hpp>
#include <graphics/contexts/impl/SmoothLightContext.hpp>

#include <config/Settings.hpp>

#include <resources/File.hpp>

namespace gfx
{

Renderer::Renderer(sf::RenderTarget & target)
    : _target(target), _current_context(0), _draw_context(0), _light_context(0)
{
    IWBAN_DEBUG(_active = false);

    // TODO Use resource manager for shaders
    // Light mix
    res::File light = res::openFile("system/light.glfs");
    std::string frag_light_str((const char*) light.getData(), light.getSize());

    if (_light_mix.loadFromMemory(frag_light_str, sf::Shader::Fragment))
    {
        _light_mix.setParameter("texture", sf::Shader::CurrentTexture);
    }
    else
        // TODO Should throw?
        IWBAN_LOG_ERROR("Failed to load light mix shader\n");

    reloadConfiguration();
}

DrawContext & Renderer::openDrawContext()
{
    BOOST_ASSERT_MSG(_active, "Cannot open a context while the Renderer is inactive");
    BOOST_ASSERT_MSG(!_draw_enabled, "Cannot open a context twice in a frame");

    BOOST_ASSERT_MSG(!_current_context || !_current_context->isOpen(),
                     "You must close the current context before opening another");

    _draw_enabled = true;
    _draw_context->open();
    _current_context = _draw_context;
    return *_draw_context;
}

LightContext & Renderer::openLightContext(const sf::Color & ambient_light)
{
    BOOST_ASSERT_MSG(_active, "Cannot open a context while the Renderer is inactive");
    BOOST_ASSERT_MSG(!_light_enabled, "Cannot open a context twice in a frame");

    BOOST_ASSERT_MSG(!_current_context || !_current_context->isOpen(),
                     "You must close the current context before opening another");

    _light_enabled = true;
    _light_context->open(ambient_light);
    _current_context = _light_context;
    return *_light_context;
}

#ifndef NDEBUG
DebugContext & Renderer::openDebugContext()
{
    BOOST_ASSERT_MSG(_active, "Cannot open a context while the Renderer is inactive");
    BOOST_ASSERT_MSG(!_debug_enabled, "Cannot open a context twice in a frame");

    BOOST_ASSERT_MSG(!_current_context || !_current_context->isOpen(),
                     "You must close the current context before opening another");

    _debug_enabled = true;
    _debug_context->open();
    _current_context = _debug_context;
    return *_debug_context;
}
#endif

void Renderer::reloadConfiguration()
{
    BOOST_ASSERT_MSG(!_active, "Cannot reload configuration while the Renderer is active");

    delete _draw_context;
    delete _light_context;

    _draw_context = new DrawContext();

    if (cfg::light_smooth)
        _light_context = new impl::SmoothLightContext();
    else
        _light_context = new impl::QuickLightContext();

    IWBAN_DEBUG(_debug_context = new DebugContext());
}

void Renderer::begin()
{
    BOOST_ASSERT_MSG(!_active, "Renderer is already active");

    _draw_enabled = false;
    _light_enabled = false;

    IWBAN_DEBUG(_debug_enabled = false);

    IWBAN_DEBUG(_active = true);
}

void Renderer::end()
{
    BOOST_ASSERT_MSG(_active, "Renderer is already inactive");

    BOOST_ASSERT_MSG(!_current_context || !_current_context->isOpen(),
                     "You must close the current context in order to finish rendering");

    if (_draw_enabled)
    {
        sf::RenderStates state(sf::BlendNone);

        sf::RectangleShape sprite(sf::Vector2f(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT));
        sprite.setTexture(&_draw_context->getTexture());

        // Lighting
        if (_light_enabled)
        {
            _light_mix.setParameter("light_map", _light_context->getTexture());
            state.shader = &_light_mix;
        }

        _target.draw(sprite, state);
    }

#ifndef NDEBUG
    if (_debug_enabled)
        _target.draw(sf::Sprite(_debug_context->getTexture()));
#endif

    IWBAN_DEBUG(_active = false);
}

}
// namespace gfx
