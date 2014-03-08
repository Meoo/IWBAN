/**
 * @file   ScreenProjector.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SCREENPROJECTOR_HPP_
#define _IWBAN_SCREENPROJECTOR_HPP_

#include <Global.hpp>

#include <graphics/Renderer.hpp>

#include <boost/noncopyable.hpp>

namespace sys
{

class Screen;

// TODO ScreenTransition(s)
class ScreenProjector : public boost::noncopyable
{
private:
    Screen * _current_screen;


public:
    // Constructor
            ScreenProjector(Screen * screen);

    // Destructor
            ~ScreenProjector();

    // Functions
    Screen * getCurrentScreen() { return _current_screen; }

    void    setScreen(Screen * screen);

    void    update();

    void    render(gfx::Renderer & renderer) const;

};
// class ScreenProjector

}
// namespace sys

#endif // _IWBAN_SCREENPROJECTOR_HPP_
