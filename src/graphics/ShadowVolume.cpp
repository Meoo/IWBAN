/**
 * @file   ShadowVolume.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <graphics/Scene.hpp>
#include <graphics/ShadowVolume.hpp>

namespace gfx
{

ShadowVolume::~ShadowVolume()
{
    if (_scene)
        _scene->remove(this);
}

void ShadowVolume::init(Scene * scene)
{
    IWBAN_PRE_PTR(scene);
    IWBAN_PRE(!_scene);
    _scene = scene;
}

void ShadowVolume::deinit()
{
    IWBAN_PRE_PTR(_scene);
    _scene = nullptr;
}


}
// namespace gfx
