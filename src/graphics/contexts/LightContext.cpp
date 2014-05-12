/**
 * @file   LightContext.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/contexts/LightContext.hpp>

#include <SFML/OpenGL.hpp>

namespace
{

// TODO Constant in config instead
const float SHADOW_FAR_DISTANCE = 2048;

inline
bool addVertex(sf::VertexArray & array,
               const ut::Vector & vertex,
               const ut::Vector & delta)
{
    float delta_len = std::abs(delta.x) + std::abs(delta.y);

    // Do not add a vertex if it's too close from origin
    if (delta_len < 1)
        return false;

    ut::Vector far_vertex = delta * (SHADOW_FAR_DISTANCE / delta_len) + vertex;

    array.append(sf::Vertex(sf::Vector2f(vertex.x, vertex.y)));
    array.append(sf::Vertex(sf::Vector2f(far_vertex.x, far_vertex.y)));

    return true;
}

inline
void popVertices(sf::VertexArray & array,
                 sf::RenderTarget & target)
{
    if (array.getVertexCount() > 2)
        target.draw(array);

    array.clear();
}

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
    // TODO Allocate vertices by default?
    sf::VertexArray vertices(sf::TrianglesStrip);

    // TODO Draw the mask we want to use
    for (const ShadowVolume * shadow : list)
    {
        if (shadow->getVertexCount() == 0)
        {
            // TODO Assert ? At least put the check in another place?
            IWBAN_LOG_ERROR("Using a shadow volume with no vertices!\n");
            continue;
        }

        ut::Vector last_delta = shadow->getVertex(0) - origin;
        ::addVertex(vertices, shadow->getVertex(0), last_delta);

        for (std::size_t i = 1; i < shadow->getVertexCount(); ++i)
        {
            ut::Vector vert = shadow->getVertex(i);
            ut::Vector delta = vert - origin;

            // Check winding
            // Do a dot product between delta and the
            if (delta.x * - last_delta.y + delta.y * last_delta.x < 0)
            {
                if (!::addVertex(vertices, vert, delta))

                    // Pop previous vertices if adding one fails
                    // It's a very rare case
                    ::popVertices(vertices, getRenderTexture());
            }
            else
            {
                ::popVertices(vertices, getRenderTexture());
                ::addVertex(vertices, vert, delta);
            }

            last_delta = delta;
        }

        ::popVertices(vertices, getRenderTexture());
    }

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
