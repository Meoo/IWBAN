/**
 * @file   LightContext.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/DisplayConfig.hpp>
#include <config/Settings.hpp>

#include <graphics/Light.hpp>
#include <graphics/ShadowVolume.hpp>
#include <graphics/contexts/LightContext.hpp>

namespace
{

// TODO Use a constant in config file instead
const float SHADOW_FAR_DISTANCE = 32768;

}
// namespace

namespace gfx
{

LightContext::LightContext()
    : _shadow_volumes(nullptr)
{
    _render_light.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT);
    _render_light_mask.create(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT);

    if (!cfg::pixelated)
        _render_light.setSmooth(true);

    _light_mix = data::getShader("system/light.gls");
}

void LightContext::draw(const gfx::Light & light)
{
    IWBAN_PRE(_open);

    sf::RenderStates state(sf::BlendAdd);

    if (/* TODO mask enabled */ false)
    {
        buildShadowMask(light);
        _light_mix->setParameter("light_map", _render_light_mask.getTexture());
        state.shader = _light_mix;
    }

    // TODO Draw light (use shadow mask if necessary)
}

void LightContext::buildShadowMask(const gfx::Light & light)
{
    if (!_shadow_volumes) return;

    _render_light_mask.clear(sf::Color::Black);

    // Keep the vertex array here to avoid many memory allocations
    // TODO Static or member data?
    static sf::VertexArray vertices(sf::Quads);

    const ut::Vector & origin   = light.getOrigin();
    const ut::Rectangle bounds  = light.getBounds();

    // Draw the mask we want to use
    for (const ShadowVolume * shadow : *_shadow_volumes)
    {
        if (!shadow->getBounds().isIntersecting(bounds))
            continue;

        bool       last_valid = false;
        ut::Vector last_vert;
        ut::Vector last_delta;
        ut::Vector last_far;

        sf::Color color = shadow->getShadowColor();

        for (std::size_t i = 0; i < shadow->getVertexCount(); ++i)
        {
            bool       valid = false;
            ut::Vector vert = shadow->getVertex(i);
            ut::Vector delta = vert - origin;
            ut::Vector far_; // "far" is used as a macro somewhere...

            {
                float delta_len = std::abs(delta.x) + std::abs(delta.y);

                // Do not add a vertex if it's too close from origin
                if (delta_len >= 1)
                {
                    far_ = delta * (SHADOW_FAR_DISTANCE / delta_len) + vert;
                    valid = true;
                }
            }

            // Check winding
            // Do a dot product between delta and the last delta rotated by 90 degrees
            if (last_valid && valid
                && delta.x * - last_delta.y + delta.y * last_delta.x < 0)
            {
                vertices.append(sf::Vertex(last_vert, color));
                vertices.append(sf::Vertex(last_far, color));
                vertices.append(sf::Vertex(far_, color));
                vertices.append(sf::Vertex(vert, color));
            }

            last_valid = valid;
            last_vert = vert;
            last_delta = delta;
            last_far = far_;
        }

        _render_light_mask.draw(vertices, sf::RenderStates(sf::BlendAdd));
        vertices.clear();
    }
}

void LightContext::open(const sf::View & view, const sf::Color & ambient_color)
{
    IWBAN_PRE(!_open);

    _render_light.setView(view);
    _render_light.clear(ambient_color);
    _shadow_volumes = nullptr;

    _open = true;
}

void LightContext::close()
{
    IWBAN_PRE(_open);

    _render_light.display();

    _open = false;
}

const sf::Texture & LightContext::getTexture() const
{
    return _render_light.getTexture();
}

}
// namespace gfx
