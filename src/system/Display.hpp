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
    // TODO Rename data members
    // Data members
    // Render window
    sf::RenderWindow    _window;

    // Icon and background data
    sf::Image           win_icon;
    sf::Texture         bg_tex;
    sf::VertexArray     bg_mesh;
    sf::View            bg_view;

    // Screen size and view data
    float               zoom;
    int                 sceneW;
    int                 sceneH;
    int                 marginX;
    int                 marginY;
    float               winW2;
    float               winH2;
    sf::View            render_view;

    // Debug variable
    IWBAN_DEBUG(bool    _ready);


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
