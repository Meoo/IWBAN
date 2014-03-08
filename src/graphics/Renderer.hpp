/**
 * @file   Renderer.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_RENDERER_HPP_
#define _IWBAN_RENDERER_HPP_

#include <Global.hpp>

#include <SFML/Graphics.hpp>

#include <boost/container/vector.hpp>

namespace gfx
{

class Renderer
{
private:
    sf::RenderTarget &  _target;

    sf::RenderTexture   _render_scene;
    sf::RenderTexture   _render_light;
    sf::RenderTexture   _render_light_inter;

    bool                _smooth_light;
    sf::Shader          _blur_filter;

    IWBAN_DEBUG(bool    _ready);
    IWBAN_DEBUG(bool    _light_ready);

public:
    // Constructor
    Renderer(sf::RenderTarget & target);

    // Destructor
    ~Renderer() {}

    // Functions
    void begin();
    void end();

    void draw(sf::Drawable & drawable,
              const sf::RenderStates & states = sf::RenderStates::Default);

    void fill(const sf::Color & color,
              const sf::RenderStates & states = sf::RenderStates::Default);

    void beginLight(const sf::Color & light_color);
    void endLight();

    void drawLight(sf::Drawable & drawable);

};
// class Renderer

}
// namespace gfx

#endif // _IWBAN_RENDERER_HPP_
