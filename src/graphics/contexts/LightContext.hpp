/**
 * @file   LightContext.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_LIGHTCONTEXT_HPP_
#define _IWBAN_LIGHTCONTEXT_HPP_

#include <Global.hpp>

#include <data/Shader.hpp>

#include <graphics/Context.hpp>
#include <graphics/ShadowVolume.hpp>

#include <utils/Vector.hpp>

#include <SFML/Graphics.hpp>

namespace gfx
{

class Light;
class ShadowVolume;

class Renderer;

class LightContext : public Context
{
private:
    // Data members
    sf::RenderTexture   _render_light;
    sf::RenderTexture   _render_light_mask;
    data::Shader        _light_mix;

    bool                _open   = false;


public:
    LightContext();

    void draw(const gfx::Light & light);

    bool isOpen() const override { return _open; }
    void close() override;


private:
    // Internal functions
    void buildShadowMask(const gfx::Light & light,
                         const std::vector<ShadowVolume *> & list);


    friend class Renderer;

    void open(const sf::View & view, const sf::Color & ambient_color);
    const sf::Texture & getTexture() const;

};
// class LightContext

}
// namespace gfx

#endif // _IWBAN_LIGHTCONTEXT_HPP_
