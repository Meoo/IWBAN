/**
 * @file   LightContext.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/contexts/LightContext.hpp>

#include <SFML/OpenGL.hpp>

namespace
{

// TODO Use a constant in config file instead
const float SHADOW_FAR_DISTANCE = 32768;

}
// namespace

namespace gfx
{

void LightContext::buildShadowMask(const ut::Vector & origin,
                                   const std::vector<ShadowVolume *> & list)
{
    getRenderTexture().setActive(true);

    // Enable writing on the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDepthMask(GL_TRUE);

    // Disable drawing
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    // Clean mask
    glClear(GL_DEPTH_BUFFER_BIT);

    // Keep the vertex array here to avoid many memory allocations
    static sf::VertexArray vertices(sf::Quads);

    // Draw the mask we want to use
    for (const ShadowVolume * shadow : list)
    {
        if (shadow->getVertexCount() == 0)
        {
            // TODO Assert ? At least put the check in another place?
            IWBAN_LOG_ERROR("Using a shadow volume with no vertices!\n");
            continue;
        }

        bool       last_valid = false;
        ut::Vector last_vert;
        ut::Vector last_delta;
        ut::Vector last_far;

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
                vertices.append(sf::Vector2f(last_vert.x, last_vert.y));
                vertices.append(sf::Vector2f(last_far.x, last_far.y));
                vertices.append(sf::Vector2f(far_.x, far_.y));
                vertices.append(sf::Vector2f(vert.x, vert.y));
            }

            last_valid = valid;
            last_vert = vert;
            last_delta = delta;
            last_far = far_;
        }
    }

    getRenderTexture().draw(vertices);
    vertices.clear();

    // Disable writing on the depth buffer and activate the mask
    glDepthFunc(GL_NOTEQUAL);
    glDepthMask(GL_FALSE);

    // Enable drawing again
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void LightContext::disableShadowMask()
{
    getRenderTexture().setActive(true);

    // Disable the mask
    glDisable(GL_DEPTH_TEST);
}

}
// namespace gfx
