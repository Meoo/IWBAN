/**
 * @file   SmoothLightContext.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SMOOTHLIGHTCONTEXT_HPP_
#define _IWBAN_SMOOTHLIGHTCONTEXT_HPP_

#include <Global.hpp>

#include <graphics/contexts/LightContext.hpp>

namespace gfx
{

namespace impl
{

class SmoothLightContext : public LightContext
{
private:
    // Data members
    sf::RenderTexture   _render_light;
    sf::RenderTexture   _render_light_inter;

    sf::Shader          _blur_h_filter;
    sf::Shader          _blur_v_filter;


public:
    SmoothLightContext();

    // Virtual functions
    virtual void draw(const sf::Drawable & drawable);


protected:
    // Protected functions for Renderer
    virtual void open(const sf::Color & ambient_light);

    virtual void close();

    virtual const sf::Texture & getTexture() const;

};
// class SmoothLightContext

}
// namespace impl

}
// namespace gfx

#endif // _IWBAN_SMOOTHLIGHTCONTEXT_HPP_
