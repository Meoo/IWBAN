/**
 * @file   Camera.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_CAMERA_HPP_
#define _IWBAN_CAMERA_HPP_

#include <Global.hpp>

#include <utils/Vector.hpp>
#include <utils/Rectangle.hpp>

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>

namespace gfx
{

class Camera
{
private:
    // Data members
    ut::Vector  _center;
    sf::View    _view;


public:
    // Constructor
                    Camera();
    explicit        Camera(const ut::Vector & center);

    // Getters / setters
    const sf::View & getView() const                { return _view; }
    ut::Rectangle   getViewBounds() const;

    void            setCenter(const ut::Vector & center);

    // Functions
    ut::Vector      screenToWorld(const ut::Vector & screen_coord) const;
    ut::Vector      worldToScreen(const ut::Vector & world_coord) const;


private:
    void            updateView();

};
// class Camera

// ---- ---- ---- ----

class CameraOperator
{
private:
    // Data members
    Camera      _camera;


public:
    // Constructor
                CameraOperator() = default;

    // Getters / setters
    const Camera & getCamera() const                { return _camera; }
    void        setCamera(const Camera & camera)    { _camera = camera; }

    Camera      getCameraAt(const sf::Time & time) const;

    // Functions
    void        setTarget(const ut::Vector & target);

};
// class CameraOperator

}
// namespace gfx

#endif // _IWBAN_CAMERA_HPP_
