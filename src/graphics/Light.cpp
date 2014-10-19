/**
 * @file   Light.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/Light.hpp>
#include <graphics/Scene.hpp>

namespace gfx
{

Light::~Light()
{
    if (_scene)
        _scene->remove(this);
}

void Light::init(Scene * scene)
{
    IWBAN_PRE_PTR(scene);
    IWBAN_PRE(!_scene);
    _scene = scene;
}

void Light::deinit()
{
    IWBAN_PRE_PTR(_scene);
    _scene = nullptr;
}


}
// namespace gfx
