/**
 * @file   Drawable.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/Drawable.hpp>
#include <graphics/Scene.hpp>

namespace gfx
{

Drawable::~Drawable()
{
    if (_scene)
        _scene->remove(this);
}

void Drawable::init(Scene * scene)
{
    IWBAN_PRE_PTR(scene);
    IWBAN_PRE(!_scene);
    _scene = scene;
}

void Drawable::deinit()
{
    IWBAN_PRE_PTR(_scene);
    _scene = nullptr;
}

}
// namespace gfx
