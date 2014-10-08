/**
 * @file   Renderer.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/DisplayConfig.hpp>
#include <config/Settings.hpp>

#include <graphics/Renderer.hpp>

namespace gfx
{

Renderer::Renderer(sf::RenderTarget & target)
    : _target(target),
      _screen_shape(sf::Vector2f(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT)),
      _overlay_color(sf::Color::Transparent),
      _color_correction(sf::Color::White)
{
#ifndef NDEBUG
    _debug_active = false;
#endif

    _screen_shape.setOrigin(IWBAN_FRAME_WIDTH / 2, IWBAN_FRAME_HEIGHT / 2);
    _screen_shape.setPosition(IWBAN_FRAME_WIDTH / 2, IWBAN_FRAME_HEIGHT / 2);

    _light_mix = data::getShader("system/light.gls");

    reloadConfiguration();
}

DrawContext & Renderer::openDrawContext()
{
    IWBAN_PRE(_debug_active);
    IWBAN_PRE_MSG(!_draw_enabled, "Cannot open a context twice in a frame");
    IWBAN_PRE_MSG(!_gui_enabled, "Cannot open draw context after gui context");

    IWBAN_PRE_MSG(!_current_context || !_current_context->isOpen(),
                  "You must close the current context before opening another");

    _draw_enabled = true;
    _draw_context->open(_camera.getView());
    _current_context = _draw_context.get();
    return *_draw_context;
}

LightContext & Renderer::openLightContext(const sf::Color & ambient_light)
{
    IWBAN_PRE(_debug_active);
    IWBAN_PRE_MSG(!_light_enabled, "Cannot open a context twice in a frame");
    IWBAN_PRE_MSG(!_gui_enabled, "Cannot open light context after gui context");

    IWBAN_PRE_MSG(!_current_context || !_current_context->isOpen(),
                  "You must close the current context before opening another");

    _light_enabled = true;
    _light_context->open(_camera.getView(), ambient_light);
    _current_context = _light_context.get();
    return *_light_context;
}

GuiContext & Renderer::openGuiContext()
{
    IWBAN_PRE(_debug_active);
    IWBAN_PRE_MSG(!_gui_enabled, "Cannot open a context twice in a frame");

    IWBAN_PRE_MSG(!_current_context || !_current_context->isOpen(),
                  "You must close the current context before opening another");

    // Flush draw and light before drawing gui
    flushDrawLight();

    _gui_enabled = true;
    _gui_context->open();
    _current_context = _gui_context.get();
    return *_gui_context;
}

#ifndef NDEBUG
DebugContext & Renderer::openDebugContext()
{
    IWBAN_PRE(_debug_active);
    IWBAN_PRE_MSG(!_debug_enabled, "Cannot open a context twice in a frame");

    IWBAN_PRE_MSG(!_current_context || !_current_context->isOpen(),
                  "You must close the current context before opening another");

    _debug_enabled = true;
    _debug_context->open(_camera.getView());
    _current_context = _debug_context.get();
    return *_debug_context;
}
#endif

void Renderer::reloadConfiguration()
{
    IWBAN_PRE(!_debug_active);

    _draw_context.reset(new DrawContext());
    _light_context.reset(new LightContext());
    _gui_context.reset(new GuiContext(_target));

#ifndef NDEBUG
    _debug_context.reset(new DebugContext());
#endif
}

void Renderer::begin()
{
    IWBAN_PRE(!_debug_active);

    _current_context = nullptr;

    _draw_enabled   = false;
    _light_enabled  = false;
    _gui_enabled    = false;


    _flushed        = false;

#ifndef NDEBUG
    _debug_enabled  = false;
    _debug_active   = true;
#endif
}

void Renderer::end()
{
    IWBAN_PRE(_debug_active);

    IWBAN_PRE_MSG(!_current_context || !_current_context->isOpen(),
                  "You must close the current context in order to finish rendering");

    flushDrawLight();

    if (_overlay_color.a != 0)
    {
        _screen_shape.setTexture(nullptr);
        _screen_shape.setFillColor(_overlay_color);

        _target.draw(_screen_shape);
    }

#ifndef NDEBUG
    if (_debug_enabled)
    {
        _screen_shape.setTexture(&_debug_context->getTexture());
        _screen_shape.setFillColor(sf::Color::White);

        _target.draw(_screen_shape);
    }

    _debug_active = false;
#endif
}

void Renderer::flushDrawLight()
{
    if (_flushed) return;

    if (_draw_enabled)
    {
        sf::RenderStates state(sf::BlendNone);

        _screen_shape.setTexture(&_draw_context->getTexture());
        _screen_shape.setFillColor(_color_correction);

        // Lighting
        if (_light_enabled)
        {
            _light_mix->setParameter("light_map", _light_context->getTexture());
            state.shader = _light_mix;
        }

        _target.draw(_screen_shape, state);
    }

    _flushed = true;
}

void Renderer::setRotation(float angle)
{
    _screen_shape.setRotation(angle);
}

void Renderer::setOffset(float x, float y)
{
    _screen_shape.setPosition(IWBAN_FRAME_WIDTH / 2 + x,
                              IWBAN_FRAME_HEIGHT / 2 + y);
}

}
// namespace gfx
