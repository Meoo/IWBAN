/**
 * @file   WorldScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_WORLDSCREEN_HPP_
#define _IWBAN_WORLDSCREEN_HPP_

#include <Global.hpp>

#include <game/World.hpp>

#include <system/Screen.hpp>

namespace sys
{

class WorldScreen : public Screen
{
private:
    game::World     _world;

protected:
    // Callbacks
    virtual void    onUpdate(const sf::Time & update_time);
    virtual void    onRender(gfx::Renderer & renderer, const sf::Time & render_time) const;

    virtual void    onShow();
    virtual void    onHide();

};
// class WorldScreen

}
// namespace sys

#endif // _IWBAN_WORLDSCREEN_HPP_
