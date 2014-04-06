/**
 * @file   QuickLightContext.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_QUICKLIGHTCONTEXT_HPP_
#define _IWBAN_QUICKLIGHTCONTEXT_HPP_

#include <Global.hpp>

#include <graphics/contexts/LightContext.hpp>

namespace gfx
{

namespace impl
{

class QuickLightContext : public LightContext
{
private:
    // Data members
    sf::RenderTexture   _render_light;


public:
    QuickLightContext();

    // Virtual functions
    virtual void draw(const sf::Drawable & drawable);


protected:
    // Protected functions for Renderer
    virtual void open(const sf::Color & ambient_light);

    virtual void close();

    virtual const sf::Texture & getTexture() const;

};
// class QuickLightContext

}
// namespace impl

}
// namespace gfx

#endif // _IWBAN_QUICKLIGHTCONTEXT_HPP_
