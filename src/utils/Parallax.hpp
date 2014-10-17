/**
 * @file   Parallax.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_PARALLAX_HPP_
#define _IWBAN_PARALLAX_HPP_

#include <Global.hpp>

#include <config/DisplayConfig.hpp>

#include <SFML/Graphics/RenderStates.hpp>

namespace ut
{

/**
 * Update given RenderStates to produce a parallax effect.
 *
 * @param scale_x Horizontal scaling.
 * @param scale_y Vertical scaling.
 * @param target RenderTarget in use.
 * @param states RenderStates to be modified.
 */
inline
void makeParallaxEffect(float scale_x, float scale_y,
                        const sf::RenderTarget & target,
                        sf::RenderStates & states)
{
    IWBAN_PRE(scale_x > 0);
    IWBAN_PRE(scale_y > 0);

    if (scale_x == 1 && scale_y == 1)
        return;

    states.transform.scale(sf::Vector2f(scale_x, scale_y),
                           target.getView().getCenter());

    states.transform.scale(1 / scale_x, 1 / scale_y,
                           IWBAN_FRAME_WIDTH / 2, IWBAN_FRAME_HEIGHT / 2);
}

}
// namespace ut

#endif // _IWBAN_PARALLAX_HPP_
