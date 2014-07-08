/**
 * @file   Screen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SCREEN_HPP_
#define _IWBAN_SCREEN_HPP_

#include <Global.hpp>

#include <graphics/Renderer.hpp>

namespace sys
{

class Projector;

class Screen
{
public:
    friend class Projector;


private:
    // Data members
    Projector * _projector      = nullptr;
    Screen *    _next_screen    = nullptr;
    bool        _disposable     = false;


public:
    // Constructor
                    Screen() = default;

    // Virtual destructor
    virtual         ~Screen() {}


protected:
    // Callbacks
    virtual void    onUpdate(const sf::Time & update_time) = 0;
    virtual void    onRender(gfx::Renderer & renderer, const sf::Time & render_time) const = 0;

    virtual void    onShow() {}
    virtual void    onHide() {}

    virtual void    onMouseMove(const ut::Vector & position) {}
    virtual void    onMouseClick(const ut::Vector & position) {}

    // Internal functions
    Projector *         getProjector()          { return _projector; }
    const Projector *   getProjector() const    { return _projector; }

    /**
     * Set next screen to be projected.
     *
     * This function must be used in onUpdate() function, and
     * using it from any other callback will have no effect.
     *
     * The screen will be changed right after the function onUpdate() returns.
     */
    void    setNextScreen(Screen * next_screen)
    {
        IWBAN_PRE_PTR(next_screen);
        _next_screen = next_screen;
    }

    /**
     * Mark this Screen to be released when the screen changes.
     *
     * @pre This function must be used after a call to setNextScreen(),
     *      or an assertion will be raised.
     */
    void    dispose()
    {
        IWBAN_PRE_PTR(_next_screen);
        _disposable = true;
    }


private:
    // Private functions for Projector
    void        show(Projector * proj)
    {
        IWBAN_PRE_PTR(proj);
        IWBAN_PRE(!_projector);

        _projector = proj;
        onShow();
    }

    void        hide()
    {
        IWBAN_PRE_PTR(_projector);

        onHide();
        _projector = nullptr;
    }

    void        resetNextScreen()       { _next_screen = nullptr; }
    Screen *    getNextScreen() const   { return _next_screen; }
    bool        isDisposable() const    { return _disposable; }

};
// class Screen

}
// namespace sys

#endif // _IWBAN_SCREEN_HPP_
