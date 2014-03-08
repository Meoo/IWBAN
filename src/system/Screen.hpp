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

class ScreenProjector;

class Screen
{
public:
    friend class ScreenProjector;


public:
    // Constructor
                    Screen() {}

    // Destructor
    virtual         ~Screen() {}


protected:
    // Callbacks
    virtual void    onUpdate() = 0;
    virtual void    onRender(gfx::Renderer & renderer) const = 0;

    virtual void    onShow() = 0;
    virtual void    onHide() = 0;

};
// class Screen

}
// namespace sys

#endif // _IWBAN_SCREEN_HPP_
