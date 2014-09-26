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

void Camera::centerOn(const ut::Vector & center)
{
    _center = center;
    updateView();
}

void Camera::moveToward(const ut::Vector & center, unsigned damping)
{
    _center = (_center * damping + center) / (damping + 1);
    updateView();
}

void Camera::updateView()
{
    _view.setCenter(_center);
}

}
// namespace gfx
