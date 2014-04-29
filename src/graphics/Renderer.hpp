/**
 * @file   Renderer.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_RENDERER_HPP_
#define _IWBAN_RENDERER_HPP_

#include <Global.hpp>

#include <graphics/contexts/DrawContext.hpp>
#include <graphics/contexts/LightContext.hpp>

#ifndef NDEBUG
#  include <graphics/contexts/debug/DebugContext.hpp>
#endif

#include <SFML/Graphics.hpp>

namespace gfx
{

class Renderer
{
private:
    // Data members
    sf::RenderTarget &  _target;

    Context *           _current_context;

    DrawContext *       _draw_context;
    bool                _draw_enabled;

    LightContext *      _light_context;
    bool                _light_enabled;

#ifndef NDEBUG
    DebugContext *      _debug_context;
    bool                _debug_enabled;
#endif

    sf::Shader          _light_mix;

    IWBAN_DEBUG(bool    _active);


public:
    // Constructor
    Renderer(sf::RenderTarget & target);

    // Functions
    DrawContext & openDrawContext();

    LightContext & openLightContext(const sf::Color & ambient_light);

#ifndef NDEBUG
    DebugContext & openDebugContext();
#endif

    void reloadConfiguration();

    void begin();

    void end();

};
// class Renderer

}
// namespace gfx

#endif // _IWBAN_RENDERER_HPP_
