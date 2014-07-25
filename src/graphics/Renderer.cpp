/**
 * @file   Renderer.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/DisplayConfig.hpp>
#include <config/Settings.hpp>

#include <graphics/Renderer.hpp>
#include <graphics/contexts/impl/QuickLightContext.hpp>
#include <graphics/contexts/impl/SmoothLightContext.hpp>

#include <resources/File.hpp>

namespace gfx
{

Renderer::Renderer(sf::RenderTarget & target)
    : _target(target), _overlay_color(sf::Color::Transparent)
{
    IWBAN_DEBUG(d_active = false);

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
        IWBAN_LOG_ERROR("Failed to load light mix shader");

    reloadConfiguration();
}

DrawContext & Renderer::openDrawContext()
{
    IWBAN_PRE(d_active);
    IWBAN_PRE_MSG(!_draw_enabled, "Cannot open a context twice in a frame");
    IWBAN_PRE_MSG(!_gui_enabled, "Cannot open draw context after gui context");

    IWBAN_PRE_MSG(!_current_context || !_current_context->isOpen(),
                  "You must close the current context before opening another");

    _draw_enabled = true;
    _draw_context->open();
    _current_context = _draw_context;
    return *_draw_context;
}

LightContext & Renderer::openLightContext(const sf::Color & ambient_light)
{
    IWBAN_PRE(d_active);
    IWBAN_PRE_MSG(!_light_enabled, "Cannot open a context twice in a frame");
    IWBAN_PRE_MSG(!_gui_enabled, "Cannot open light context after gui context");

    IWBAN_PRE_MSG(!_current_context || !_current_context->isOpen(),
                  "You must close the current context before opening another");

    _light_enabled = true;
    _light_context->open(ambient_light);
    _current_context = _light_context;
    return *_light_context;
}

GuiContext & Renderer::openGuiContext()
{
    IWBAN_PRE(d_active);
    IWBAN_PRE_MSG(!_gui_enabled, "Cannot open a context twice in a frame");

    IWBAN_PRE_MSG(!_current_context || !_current_context->isOpen(),
                  "You must close the current context before opening another");

    // Flush draw and light before drawing gui
    flushDrawLight();

    _gui_enabled = true;
    _gui_context->open();
    _current_context = _gui_context;
    return *_gui_context;
}

#ifndef NDEBUG
DebugContext & Renderer::openDebugContext()
{
    IWBAN_PRE(d_active);
    IWBAN_PRE_MSG(!_debug_enabled, "Cannot open a context twice in a frame");

    IWBAN_PRE_MSG(!_current_context || !_current_context->isOpen(),
                  "You must close the current context before opening another");

    _debug_enabled = true;
    _debug_context->open();
    _current_context = _debug_context;
    return *_debug_context;
}
#endif

void Renderer::reloadConfiguration()
{
    IWBAN_PRE(!d_active);

    delete _draw_context;
    delete _light_context;
    delete _gui_context;

    _draw_context = new DrawContext();

    if (cfg::light_smooth)
        _light_context = new impl::SmoothLightContext();
    else
        _light_context = new impl::QuickLightContext();

    _gui_context = new GuiContext(_target);

    IWBAN_DEBUG(_debug_context = new DebugContext());
}

void Renderer::begin()
{
    IWBAN_PRE(!d_active);

    _current_context = nullptr;

    _draw_enabled   = false;
    _light_enabled  = false;
    _gui_enabled    = false;

    IWBAN_DEBUG(_debug_enabled = false);

    _flushed        = false;

    IWBAN_DEBUG(d_active = true);
}

void Renderer::end()
{
    IWBAN_PRE(d_active);

    IWBAN_PRE_MSG(!_current_context || !_current_context->isOpen(),
                  "You must close the current context in order to finish rendering");

    flushDrawLight();

    if (_overlay_color.a != 0)
    {
        sf::RectangleShape sprite(sf::Vector2f(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT));
        sprite.setFillColor(_overlay_color);
        _target.draw(sprite);
    }

#ifndef NDEBUG
    if (_debug_enabled)
        _target.draw(sf::Sprite(_debug_context->getTexture()));
#endif

    IWBAN_DEBUG(d_active = false);
}

void Renderer::flushDrawLight()
{
    if (_flushed) return;

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

    _flushed = true;
}

}
// namespace gfx
