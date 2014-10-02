/**
 * @file   Camera.hpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <config/DisplayConfig.hpp>
#include <graphics/Camera.hpp>

namespace gfx
{

Camera::Camera()
    : _center(ut::Vector(IWBAN_FRAME_WIDTH / 2, IWBAN_FRAME_HEIGHT / 2))
    , _view(ut::Vector(), ut::Vector(IWBAN_FRAME_WIDTH, IWBAN_FRAME_HEIGHT))
{
    updateView();
}

void Camera::setCenter(const ut::Vector & center)
{
    _center = center;
    updateView();
}

ut::Vector Camera::screenToWorld(const ut::Vector & screen_coord) const
{
    return screen_coord + _center - ut::Vector(IWBAN_FRAME_WIDTH / 2, IWBAN_FRAME_HEIGHT / 2);
}

ut::Vector Camera::worldToScreen(const ut::Vector & world_coord) const
{
    return world_coord - _center + ut::Vector(IWBAN_FRAME_WIDTH / 2, IWBAN_FRAME_HEIGHT / 2);
}

void Camera::updateView()
{
    _view.setCenter(_center);
}

// ---- ---- ---- ----

Camera CameraOperator::getCameraAt(const sf::Time & time) const
{
    return _camera;
}

void CameraOperator::setTarget(const ut::Vector & target)
{
    int x = (((unsigned) target.x) / IWBAN_FRAME_WIDTH) * IWBAN_FRAME_WIDTH + IWBAN_FRAME_WIDTH / 2;
    int y = (((unsigned) target.y) / IWBAN_FRAME_HEIGHT) * IWBAN_FRAME_HEIGHT + IWBAN_FRAME_HEIGHT / 2;
    _camera.setCenter(ut::Vector(x, y));
}

}
// namespace gfx
