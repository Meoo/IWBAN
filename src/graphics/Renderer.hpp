/**
 * @file   Renderer.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_RENDERER_HPP_
#define _IWBAN_RENDERER_HPP_

#include <Global.hpp>

#include <data/Shader.hpp>

#include <graphics/Camera.hpp>
#include <graphics/contexts/DrawContext.hpp>
#include <graphics/contexts/GuiContext.hpp>
#include <graphics/contexts/LightContext.hpp>

#ifndef NDEBUG
#  include <graphics/contexts/debug/DebugContext.hpp>
#endif

#include <SFML/Graphics.hpp>

#include <memory>

namespace gfx
{

class Renderer
{
private:
    // Data members
    sf::RenderTarget &  _target;

    Camera              _camera;

    Context *           _current_context;

    std::unique_ptr<DrawContext>    _draw_context;
    bool                            _draw_enabled;

    std::unique_ptr<LightContext>   _light_context;
    bool                            _light_enabled;

    std::unique_ptr<GuiContext>     _gui_context;
    bool                            _gui_enabled;

#ifndef NDEBUG
    std::unique_ptr<DebugContext>   _debug_context;
    bool                            _debug_enabled;
#endif

    sf::RectangleShape  _screen_shape;

    bool                _flushed;

    data::Shader        _light_mix;
    sf::Color           _overlay_color;
    sf::Color           _color_correction;

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

    void setOverlayColor(const sf::Color & color)   { _overlay_color = color; }
    void setColorCorrection(const sf::Color & color){ _color_correction = color; }

    void setRotation(float angle);
    void setOffset(float x, float y);

          Camera & getCamera()                      { return _camera; }
    const Camera & getCamera() const                { return _camera; }

    void setCamera(const Camera & camera)           { _camera = camera; }


private:
    // Flush draw and light contexts to the screen
    void flushDrawLight();

};
// class Renderer

}
// namespace gfx

#endif // _IWBAN_RENDERER_HPP_
