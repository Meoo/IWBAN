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

    bool                _open;


public:
    SmoothLightContext();

    // Virtual functions
    virtual void draw(const sf::Drawable & drawable);

    virtual bool isOpen() const { return _open; }

    virtual void close();


protected:
    // Protected functions for Renderer
    virtual void open(const sf::Color & ambient_light);

    virtual const sf::Texture & getTexture() const;

    // Used internally to build shadow mask
    virtual sf::RenderTexture & getRenderTexture() { return _render_light; }

};
// class SmoothLightContext

}
// namespace impl

}
// namespace gfx

#endif // _IWBAN_SMOOTHLIGHTCONTEXT_HPP_
