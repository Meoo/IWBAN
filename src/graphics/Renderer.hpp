/**
 * @file   Renderer.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_RENDERER_HPP_
#define _IWBAN_RENDERER_HPP_

#include <Global.hpp>

#include <graphics/contexts/DrawContext.hpp>
#include <graphics/contexts/GuiContext.hpp>
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

    DrawContext *       _draw_context = nullptr;
    bool                _draw_enabled;

    LightContext *      _light_context = nullptr;
    bool                _light_enabled;

    GuiContext *        _gui_context = nullptr;
    bool                _gui_enabled;

#ifndef NDEBUG
    DebugContext *      _debug_context = nullptr;
    bool                _debug_enabled;
#endif

    bool                _flushed;

    sf::Shader          _light_mix;
    sf::Color           _overlay_color;

    IWBAN_DEBUG(bool    d_active);


public:
    // Constructor
    Renderer(sf::RenderTarget & target);

    // Functions
    DrawContext &   openDrawContext();

    LightContext &  openLightContext(const sf::Color & ambient_light);

    // Gui context must be opened last
    GuiContext &    openGuiContext();

#ifndef NDEBUG
    // Debug context can be opened at any time (even after Gui)
    DebugContext &  openDebugContext();
#endif

    void reloadConfiguration();

    void begin();

    void end();

    void setOverlayColor(const sf::Color & color) { _overlay_color = color; }


private:
    // Flush draw and light contexts to the screen
    void flushDrawLight();

};
// class Renderer

}
// namespace gfx

#endif // _IWBAN_RENDERER_HPP_
