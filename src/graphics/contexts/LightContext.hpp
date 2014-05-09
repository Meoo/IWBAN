/**
 * @file   LightContext.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LIGHTCONTEXT_HPP_
#define _IWBAN_LIGHTCONTEXT_HPP_

#include <Global.hpp>

#include <graphics/Context.hpp>
#include <graphics/ShadowVolume.hpp>

#include <utils/Vector.hpp>

#include <SFML/Graphics.hpp>

namespace gfx
{

class Renderer;

class LightContext : public Context
{
public:
    friend class Renderer;


public:
    // Virtual destructor
    virtual ~LightContext() {}

    // Virtual functions
    virtual void draw(const sf::Drawable & drawable) = 0;

    // Functions
    void buildShadowMask(const ut::Vector & origin,
                         const std::vector<ShadowVolume *> & list);

    void disableShadowMask();


protected:
    // Protected functions for Renderer
    virtual void open(const sf::Color & ambient_light) = 0;

    virtual const sf::Texture & getTexture() const = 0;

    // Used internally to build shadow mask
    virtual sf::RenderTexture & getRenderTexture() = 0;

};
// class LightContext

}
// namespace gfx

#endif // _IWBAN_LIGHTCONTEXT_HPP_
