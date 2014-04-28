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
    Screen *    _next_screen;
    bool        _disposable;


public:
    // Constructor
                    Screen()
                        : _next_screen(0), _disposable(false)
                    {}

    // Destructor
    virtual         ~Screen() {}


protected:
    // Callbacks
    virtual void    onUpdate() = 0;
    virtual void    onRender(gfx::Renderer & renderer) const = 0;

    virtual void    onShow() = 0;
    virtual void    onHide() = 0;

    // Internal functions
    /**
     * Set next screen to be projected.
     *
     * This function must be used in onUpdate() function, and
     * using it from any other callback will have no effect.
     *
     * The screen will be changed right after the function onUpdate() returns.
     */
    void    setNextScreen(Screen * next_screen)
        { BOOST_ASSERT(next_screen); _next_screen = next_screen; }

    /**
     * Mark this Screen to be released when the screen changes.
     *
     * @pre This function must be used after a call to setNextScreen(),
     *      or an assertion will be raised.
     */
    void    dispose()
        { BOOST_ASSERT(_next_screen); _disposable = true; }


private:
    // Functions for Projector
    Screen *    getNextScreen() const   { return _next_screen; }
    bool        isDisposable() const    { return _disposable; }

};
// class Screen

}
// namespace sys

#endif // _IWBAN_SCREEN_HPP_
