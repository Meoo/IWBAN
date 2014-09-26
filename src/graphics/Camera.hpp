/**
 * @file   Camera.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CAMERA_HPP_
#define _IWBAN_CAMERA_HPP_

#include <Global.hpp>

#include <utils/Vector.hpp>

#include <SFML/Graphics/View.hpp>

namespace gfx
{

class Camera
{
private:
    // Data members
    ut::Vector  _center;
    sf::View    _view;


public:
                Camera();

    void        centerOn(const ut::Vector & center);

    void        moveToward(const ut::Vector & center, unsigned damping = 16);

    const sf::View & getView() const { return _view; }


private:
    void        updateView();

};
// class Camera

}
// namespace gfx

#endif // _IWBAN_CAMERA_HPP_
