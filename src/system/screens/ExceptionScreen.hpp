/**
 * @file   ExceptionScreen.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_EXCEPTIONSCREEN_HPP_
#define _IWBAN_EXCEPTIONSCREEN_HPP_

#include <Global.hpp>

#include <system/Screen.hpp>

namespace sys
{

class ExceptionScreen : public Screen
{
private:
    // Data member
    Screen * _previous_screen;


public:
    // Constructor
    explicit ExceptionScreen(Screen * previous_screen)
        : _previous_screen(previous_screen)
    {}

    virtual ~ExceptionScreen()
    {
        delete _previous_screen;
    }


protected:
    // Callbacks
    virtual void    onUpdate() {}
    virtual void    onRender(gfx::Renderer & renderer) const {}

    virtual void    onShow() {}
    virtual void    onHide() {}

};
// class IntroScreen

}
// namespace sys

#endif // _IWBAN_EXCEPTIONSCREEN_HPP_
