/**
 * @file   LightContext.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LIGHTCONTEXT_HPP_
#define _IWBAN_LIGHTCONTEXT_HPP_

#include <Global.hpp>

#include <data/Shader.hpp>
#include <graphics/Context.hpp>
#include <utils/Vector.hpp>

#include <SFML/Graphics/RenderTexture.hpp>

#include <list>

namespace gfx
{

class Light;
class ShadowVolume;

class Renderer;

class LightContext : public Context
{
public:
    typedef std::list<ShadowVolume *> ShadowVolumeList;


private:
    // Data members
    sf::RenderTexture   _render_light;
    sf::RenderTexture   _render_light_mask;
    data::Shader        _light_mix;

    const ShadowVolumeList * _shadow_volumes;

    bool                _open   = false;


public:
    // Constructor
            LightContext();

    // Functions
    void    draw(const gfx::Light & light);

    bool    isOpen() const override { return _open; }
    void    close() override;

    /**
     * Set ShadowVolume list to be used when drawing Lights.
     *
     * The list must remain valid while the context is open.
     * The content of the list may be changed in between calls to the #draw function.
     *
     * Ideally, call this function only once after opening the context and don't touch the list.
     *
     * @param shadow_list New shadow list, or a null pointer.
     */
    void    setShadowVolumeList(const ShadowVolumeList * shadow_list) { _shadow_volumes = shadow_list; }


private:
    void    buildShadowMask(const gfx::Light & light);

    friend class Renderer;

    void    open(const sf::View & view, const sf::Color & ambient_color);
    const sf::Texture & getTexture() const;

};
// class LightContext

}
// namespace gfx

#endif // _IWBAN_LIGHTCONTEXT_HPP_
