/**
 * @file   Display.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_DISPLAY_HPP_
#define _IWBAN_DISPLAY_HPP_

#include <Global.hpp>

#include <SFML/Graphics.hpp>

namespace sys
{

class Projector;

class Display
{
private:
    // Data members
    // Render window
    sf::RenderWindow    _window;

    // Icon and background data
    sf::Image           _window_icon;
    sf::Texture         _background_texture;
    sf::VertexArray     _background_mesh;
    sf::View            _background_view;

    // Screen size and view data
    int                 _scene_width    = 0;
    int                 _scene_height   = 0;
    float               _half_width     = 0;
    float               _half_height    = 0;
    sf::View            _render_view;

    // Debug variable
#ifndef NDEBUG
    bool                _debug_ready;
#endif


public:
    // Constructor
         Display();

    // Functions
    void open();

    void close();

    void run(Projector & projector);


private:
    void updateSceneView();

};
// class Display

}
// namespace sys

#endif // _IWBAN_DISPLAY_HPP_
