/**
 * @file   Projector.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_PROJECTOR_HPP_
#define _IWBAN_PROJECTOR_HPP_

#include <Global.hpp>

#include <graphics/Renderer.hpp>

#include <boost/noncopyable.hpp>

namespace sys
{

class Screen;

// TODO ScreenTransition(s)
class Projector : public boost::noncopyable
{
private:
    Screen *    _current_screen;

    // TODO mutable... :(
    mutable bool _fade_enabled;
    bool        _fade_in;
    sf::Color   _fade_color;
    sf::Time    _fade_start_time;
    sf::Time    _fade_delay;


public:
    // Constructor
            Projector(Screen * screen);

    // Destructor
            ~Projector();

    // Functions
    Screen * getCurrentScreen() { return _current_screen; }

    void    setScreen(Screen * screen);

    void    update(const sf::Time & update_time);
    void    render(gfx::Renderer & renderer, const sf::Time & render_time) const;

    void    mouseMove(const ut::Vector & position);
    void    mouseClick(const ut::Vector & position);

    void    fadeIn(const sf::Color & color, const sf::Time & delay);
    void    fadeOut(const sf::Color & color, const sf::Time & delay);

    /**
     * Force the fade to be cleared even if it is already in progress.
     */
    void    clearFade();

};
// class Projector

}
// namespace sys

#endif // _IWBAN_PROJECTOR_HPP_
